#pragma once
#include "Event.h"
#include "Serialization.h"
#include "Sampler.h"
#include "MemoryPool.h"
#include "Concurrency.h"
#include "ETW.h"

#include <memory>
#include <atomic>

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ScopeHeader
{
	EventTime event;
	uint32_t boardNumber{ 0 };
	uint32_t threadNumber{ 0 };
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator << ( OutputDataStream& stream, const ScopeHeader& ob);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ScopeData
{
	ScopeHeader header;
	std::vector<EventData> categories;
	std::vector<EventTime> synchronization;
	std::vector<EventData> events;

	void AddEvent(const EventData& data)
	{
		events.push_back(data);
		if (data.description->color != Color::Null)
		{
			if (data.description->color == Color::White)
				synchronization.push_back(data);
			else
				categories.push_back(data);
		}
	}

	size_t AddSynchronization(const std::vector<EventTime>& syncData, size_t startIndex)
	{
		for (;startIndex < syncData.size(); ++startIndex)
		{
			if (syncData[startIndex].start > header.event.finish)
				break;

			if (syncData[startIndex].start >= header.event.start && syncData[startIndex].finish <= header.event.finish)
				synchronization.push_back(syncData[startIndex]);
		}
		return startIndex;
	}

	void InitRootEvent(const EventData& data)
	{
		header.event = data;
		AddEvent(data);
	}

	void Send();
	void Clear();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OutputDataStream& operator << ( OutputDataStream& stream, const ScopeData& ob);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef MemoryPool<EventData, 1024> EventBuffer;
typedef MemoryPool<const EventData*, 32> CategoryBuffer;
typedef MemoryPool<EventTime, 1024> SynchronizationBuffer;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct EventStorage
{
	EventBuffer eventBuffer;
	CategoryBuffer categoryBuffer; 
	SynchronizationBuffer synchronizationBuffer;

	std::atomic<uint> isSampling{ 0 };

	BRO_INLINE EventData& NextEvent() 
	{
		return eventBuffer.Add(); 
	}

	BRO_INLINE void RegisterCategory(const EventData& eventData) 
	{ 
		categoryBuffer.Add() = &eventData;
	}

	// Free all temporary memory
	void Clear(bool preserveContent)
	{
		eventBuffer.Clear(preserveContent);
		categoryBuffer.Clear(preserveContent);
		synchronizationBuffer.Clear(preserveContent);
	}

	void Reset()
	{
		Clear(true);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ThreadEntry
{
	ThreadDescription description;
	EventStorage storage;
	EventStorage** threadTLS;

	ThreadEntry(const ThreadDescription& desc, EventStorage** tls) : description(desc), threadTLS(tls) {}
	void Activate(bool isActive);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Core
{
	CriticalSection lock;

	uint32_t mainThreadID;

	/// List of all threads using the profiler
	std::vector<std::unique_ptr<ThreadEntry>> threads;

	int64_t progressReportedLastTimestampMS;

	std::vector<EventTime> frames;

	void UpdateEvents();
	void Update();

	Core();

	static Core notThreadSafeInstance;

	void DumpCapturingProgress();
	void SendHandshakeResponse(ETW::Status status);
public:
	void Activate(bool active);
	bool isActive;

	// Active Frame (is used as buffer)
	static BRO_THREAD_LOCAL EventStorage* storage;

	// Controls sampling routine
	Sampler sampler;

	// Event Trace for Windows interface
	ETW etw;

	// Returns thread collection
	const std::vector<std::unique_ptr<ThreadEntry>>& GetThreads() const;

	// Starts sampling process
	void StartSampling();

	// Serialize and send current profiling progress
	void DumpProgress(const char* message = "");

	// Too much time from last report
	bool IsTimeToReportProgress() const;

	// Serialize and send frames
	void DumpFrames();

	// Serialize and send sampling data
	void DumpSamplingData();

	// Registers thread and create EventStorage
	bool RegisterThread(const ThreadDescription& description);

	// NOT Thread Safe singleton (performance)
	static BRO_INLINE Core& Get() { return notThreadSafeInstance; }

	// Main Update Function
	static void NextFrame() { Get().Update(); }

	// Get Active ThreadID
	static BRO_INLINE uint32_t GetThreadID() { return Get().mainThreadID; }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
