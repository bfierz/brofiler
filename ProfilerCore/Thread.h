#pragma once
#include "Common.h"
#include <thread>

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
class SystemSyncEvent
{
private:
	uint64_t eventHandlerMutex[5];
	uint64_t eventHandler[6];
public:
	SystemSyncEvent();
	~SystemSyncEvent();

	void Notify();
	bool WaitForEvent( int millisecondsTimeout );
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
