#pragma once
#include "Common.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CalculateCurrentThreadID();
HANDLE GetThreadHandleByThreadID(DWORD threadId);
void ReleaseThreadHandle(HANDLE threadId);
bool PauseThread(HANDLE threadId);
bool ContinueThread(HANDLE threadId);
bool RetrieveThreadContext(HANDLE threadHandle, CONTEXT& context);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SystemThread
{
public:
	SystemThread() : threadId(0) {}

	bool Create( DWORD WINAPI Action( LPVOID lpParam ), LPVOID lpParam );
	bool Terminate();
	bool Join();

	operator bool() const { return threadId != 0; }
private:
	uint64_t threadId;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ThreadSleep(DWORD milliseconds);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AtomicIncrement(volatile uint* value);
void AtomicDecrement(volatile uint* value);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SystemSyncEvent
{
private:
	uint64_t eventHandlerMutex[4];
	uint64_t eventHandler[4];
public:
	SystemSyncEvent();
	~SystemSyncEvent();

	void Notify();
	bool WaitForEvent( int millisecondsTimeout );
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}