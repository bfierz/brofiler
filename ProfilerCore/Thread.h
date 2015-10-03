#pragma once
#include "Common.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD CurrentThreadID();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SystemThread
{
public:
	SystemThread() : threadId(0) {}

	bool Create( DWORD WINAPI Action( LPVOID lpParam ), LPVOID lpParam );
	void Terminate();

	operator bool() { return threadId != 0; }
private:
	uint64 threadId;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ThreadSleep(DWORD milliseconds);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AtomicIncrement(volatile uint* value);
void AtomicDecrement(volatile uint* value);


}