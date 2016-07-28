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
}
