#pragma once
#include "Common.h"
#include <thread>
#include <mutex>
#include <condition_variable>

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
	std::mutex eventHandlerMutex;
	std::condition_variable eventHandler;
public:
	void Notify();
	bool WaitForEvent( int millisecondsTimeout );
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
