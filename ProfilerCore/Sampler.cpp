#include "Common.h"
#include "Event.h"
#include "Core.h"
#include "Serialization.h"
#include "Sampler.h"
#include "SymEngine.h"
#include <unordered_set>
#include "HPTimer.h"
#include "Hook.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CallStackTreeNode
{
	DWORD64 dwArddress;
	uint32 invokeCount;

	std::list<CallStackTreeNode> children;

	CallStackTreeNode()									: dwArddress(0), invokeCount(0) {}
	CallStackTreeNode(DWORD64 address)	: dwArddress(address), invokeCount(0) {} 

	bool Merge(const CallStack& callstack, size_t index)
	{
		++invokeCount;
		if (index == 0)
			return true;

		// I suppose, that usually sampling function has only several children.. so linear search will be fast enough
		DWORD64 address = callstack[index];
		for (auto it = children.begin(); it != children.end(); ++it)
			if (it->dwArddress == address)
				return it->Merge(callstack, index - 1);

		// Didn't find node => create one
		children.push_back(CallStackTreeNode(address));
		return children.back().Merge(callstack, index - 1); 
	}

	void CollectAddresses(std::unordered_set<DWORD64>& addresses) const
	{
		addresses.insert(dwArddress);
		for (const auto& node : children)
		{
			node.CollectAddresses(addresses);
		}
	}

	OutputDataStream& Serialize(OutputDataStream& stream) const
	{
		stream << (uint64)dwArddress << invokeCount;

		stream << (uint32)children.size();
		for (const CallStackTreeNode& node : children)
		{
			node.Serialize(stream);
		}

		return stream;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Sampler::StopSampling()
{
	if (!IsActive())
		return false;

	finishEvent.Notify();
	
	bool result = workerThread.Join();
	BRO_UNUSED(result);
	BRO_ASSERT(result, "Can't stop sampling thread!");
	
	result = workerThread.Terminate();
	BRO_UNUSED(result);
	BRO_ASSERT(result, "Can't stop sampling thread!");

	targetThreads.clear();

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Sampler::Sampler() : intervalMicroSeconds(300)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Sampler::~Sampler()
{
	StopSampling();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sampler::StartSampling(const std::vector<std::unique_ptr<ThreadEntry>>& threads, uint samplingInterval)
{
	symEngine.Init();

	intervalMicroSeconds = samplingInterval;
	targetThreads.resize(threads.size());
	std::transform(std::begin(threads), std::end(threads), std::begin(targetThreads), [](auto& thread_ptr)
	{
		return thread_ptr.get();
	});

	if (IsActive())
		StopSampling();

	callstacks.clear();

	workerThread.Create( &Sampler::AsyncUpdate, this );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Sampler::IsActive() const
{
	return (bool)workerThread;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI Sampler::AsyncUpdate(LPVOID lpParam)
{
	Sampler& sampler = *(Sampler*)(lpParam);

	std::vector<std::pair<HANDLE, ThreadEntry*>> openThreads;
	openThreads.reserve(sampler.targetThreads.size());

	for (ThreadEntry* entry : sampler.targetThreads)
	{
		DWORD threadID = entry->description.threadID;
		BRO_VERIFY(threadID != GetCurrentThreadId(), "It's a bad idea to sample specified thread! Deadlock will occur!", continue);

		HANDLE hThread = GetThreadHandleByThreadID(threadID);
		if (hThread == 0)
			continue;

		openThreads.push_back(std::make_pair(hThread, entry));
	}

	if (openThreads.empty())
		return 0;

	CallStackBuffer buffer;

	CONTEXT context;

	while ( sampler.finishEvent.WaitForEvent(sampler.intervalMicroSeconds) )
	{
		// Check whether we are inside sampling scope
		for (const auto& entry : openThreads)
		{
			HANDLE handle = entry.first;
			const ThreadEntry* thread = entry.second;

			if (!thread->storage.isSampling)
				continue;

			uint count = 0;

			if (PauseThread(handle))
			{
				// Check scope again because it is possible to leave sampling scope while trying to suspend main thread
				if (thread->storage.isSampling && RetrieveThreadContext(handle, context))
				{
					count = sampler.symEngine.GetCallstack(handle, context, buffer);
				}
				ContinueThread(handle);
			}

			if (count > 0)
			{
				sampler.callstacks.push_back(CallStack(buffer.begin(), buffer.begin() + count));
			}
		}
	}

	for (const auto& entry : openThreads)
	{
		ReleaseThreadHandle(entry.first);
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator<<(OutputDataStream& os, const Symbol * const symbol)
{
	BRO_VERIFY(symbol, "Can't serialize NULL symbol!", return os);
	return os << (uint64)symbol->address << symbol->module << symbol->function << symbol->file << symbol->line;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& Sampler::Serialize(OutputDataStream& stream)
{
	BRO_VERIFY(!IsActive(), "Can't serialize active Sampler!", return stream);

	stream << (uint32)callstacks.size();

	CallStackTreeNode tree;

	Core::Get().DumpProgress("Merging CallStacks...");

	for (const CallStack& callstack : callstacks)
	{
		if (!callstack.empty())
		{
			tree.Merge(callstack, callstack.size() - 1);
		}
	}

	std::unordered_set<DWORD64> addresses;
	tree.CollectAddresses(addresses);

	Core::Get().DumpProgress("Resolving Symbols...");

	std::vector<const Symbol*> symbols;
	for (DWORD64 address : addresses)
	{
		if (const Symbol * const symbol = symEngine.GetSymbol(address))
		{
			symbols.push_back(symbol);
		}
	}

	stream << symbols;

	tree.Serialize(stream);

	// Clear temporary data for dbghelp.dll (http://microsoft.public.windbg.narkive.com/G2WkSt2k/stackwalk64-performance-problems)
	//symEngine.Close();

	return stream;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Sampler::IsSamplingScope() const
{
	for (const ThreadEntry* entry : targetThreads)
	{
		if (const EventStorage* storage = *entry->threadTLS)
		{
			if (storage->isSampling)
			{
				return true;
			}
		}
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
size_t Sampler::GetCollectedCount() const
{
	return callstacks.size();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Sampler::SetupHook(uint64 address, bool isHooked)
{
	if (!isHooked && address == 0)
	{
		return Hook::inst.ClearAll();
	} 
	else
	{
		if (const Symbol * const symbol = symEngine.GetSymbol(address))
		{
			if (isHooked)
			{
				std::vector<ulong> threadIDs;

				const auto& threads = Core::Get().GetThreads();
				for (const auto& thread : threads)
				{
					threadIDs.push_back(thread->description.threadID);
				}

				return Hook::inst.Install(*symbol, threadIDs);
			}
			else
			{
				return Hook::inst.Clear(*symbol);
			}
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}