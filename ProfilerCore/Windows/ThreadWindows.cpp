#include "../Thread.h"
#include "../HPTimer.h"

namespace Profiler
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CalculateCurrentThreadID()
{
	return GetCurrentThreadId();
}

HANDLE GetThreadHandleByThreadID(DWORD threadId)
{
	static_assert(NULL == 0, "Silly check, but otherwise bad assumptions may be made. Idiot check");
	return OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
}

void ReleaseThreadHandle(HANDLE threadId)
{
	CloseHandle(threadId);
}

bool PauseThread(HANDLE threadId)
{
	return ((DWORD)-1) != SuspendThread(threadId);
}

bool ContinueThread(HANDLE threadId)
{
	return ((DWORD)-1) != ResumeThread(threadId);
}

bool RetrieveThreadContext(HANDLE threadHandle, CONTEXT& context)
{
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	return TRUE == GetThreadContext(threadHandle, &context);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AtomicIncrement(volatile uint* value)
{
	InterlockedIncrement(value);
}

void AtomicDecrement(volatile uint* value)
{
	InterlockedDecrement(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SystemSyncEvent::SystemSyncEvent()
{
	eventHandler[0] = (uint64_t)CreateEvent(NULL, false, false, 0);
}

SystemSyncEvent::~SystemSyncEvent()
{
	CloseHandle((HANDLE)eventHandler[0]);
	eventHandler[0] = 0;
}
	
void SystemSyncEvent::Notify()
{
	SetEvent((HANDLE)eventHandler[0]);
}

bool SystemSyncEvent::WaitForEvent( int millisecondsTimeout )
{
	if (WaitForSingleObject((HANDLE)eventHandler[0], 0) == WAIT_TIMEOUT)
	{
		SpinSleep(millisecondsTimeout);
		return true;
	}
	else
	{
		return false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
