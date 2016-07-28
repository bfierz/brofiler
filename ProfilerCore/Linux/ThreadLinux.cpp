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
}
