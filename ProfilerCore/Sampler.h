#pragma once
#include "SymEngine.h"
#include "Thread.h"

// C++ standard library
#include <array>
#include <list>
#include <memory>
#include <vector>

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OutputDataStream;
struct ThreadEntry;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Sampler
{
	SymEngine symEngine;

	std::list<CallStack> callstacks;
	std::vector<ThreadEntry*> targetThreads;

	SystemThread workerThread;
	SystemSyncEvent finishEvent;

	uint intervalMicroSeconds;

	// Called from worker thread
	static DWORD WINAPI AsyncUpdate( LPVOID lpParam );
public:
	Sampler();
	~Sampler();

	bool IsSamplingScope() const;

	bool IsActive() const;

	void StartSampling(const std::vector<std::unique_ptr<ThreadEntry>>& threads, uint samplingInterval = 300);
	bool StopSampling();

	bool SetupHook(uint64_t address, bool isHooked);

	size_t GetCollectedCount() const;
	OutputDataStream& Serialize(OutputDataStream& stream);

	static uint GetCallstack(HANDLE hThread, CONTEXT& context, CallStackBuffer& callstack);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}