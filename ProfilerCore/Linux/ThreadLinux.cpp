#include "../Thread.h"
#include <pthread.h>
#include <ctime>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <signal.h>
#include <cstring>

namespace Profiler
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CalculateCurrentThreadID()
{
	//static_assert(sizeof(DWORD) >= sizeof(pthread_t), "Information will be lost otherwise");
	return (DWORD)pthread_self();	
}

HANDLE GetThreadHandleByThreadID(DWORD threadId)
{
	static_assert(sizeof(HANDLE) >= sizeof(DWORD), "Information will be lost otherwise");
	return threadId;
}

void ReleaseThreadHandle(HANDLE threadId)
{
}

bool PauseThread(HANDLE threadId)
{
	return 0 == pthread_kill(threadId, SIGSTOP);
}

bool ContinueThread(HANDLE threadId)
{
	return 0 == pthread_kill(threadId, SIGCONT);
}

bool RetrieveThreadContext(HANDLE threadHandle, CONTEXT& context)
{
	memset(&context, 0, sizeof(CONTEXT));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemSyncEvent::SystemSyncEvent()
{
	static_assert(sizeof(std::condition_variable) <= sizeof(eventHandler) , "Increase size of eventHandler");
	static_assert(sizeof(std::mutex) <= sizeof(eventHandlerMutex), "Increase size of eventHandlerMutex");
	new (reinterpret_cast<void*>(eventHandlerMutex)) std::mutex;
	new (reinterpret_cast<void*>(eventHandler)) std::condition_variable;
}

SystemSyncEvent::~SystemSyncEvent()
{
	reinterpret_cast<std::mutex*>(&eventHandlerMutex[0])->~mutex();
	reinterpret_cast<std::condition_variable*>(&eventHandler[0])->~condition_variable();
}
	
void SystemSyncEvent::Notify()
{
	std::condition_variable *event = reinterpret_cast<std::condition_variable*>(&eventHandler[0]);
	event->notify_all();
}

bool SystemSyncEvent::WaitForEvent( int millisecondsTimeout )
{
	std::unique_lock<std::mutex> mutexLock(*reinterpret_cast<std::mutex*>(&eventHandlerMutex[0]));
	std::condition_variable *event = reinterpret_cast<std::condition_variable*>(&eventHandler[0]);
	if( std::cv_status::no_timeout == event->wait_for(mutexLock, std::chrono::milliseconds(millisecondsTimeout) ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
