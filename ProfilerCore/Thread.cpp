#include "Thread.h"
#include <ctime>
#include <thread>
#include <chrono>
#include <signal.h>
#include <cstring>

namespace Profiler
{

void SystemSyncEvent::Notify()
{
	eventHandler.notify_all();
}

bool SystemSyncEvent::WaitForEvent( int millisecondsTimeout )
{
	std::unique_lock<std::mutex> mutexLock(eventHandlerMutex);
	if(std::cv_status::no_timeout == eventHandler.wait_for(mutexLock, std::chrono::milliseconds(millisecondsTimeout)))
		return false;
	else
		return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
