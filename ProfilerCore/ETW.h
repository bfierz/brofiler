#pragma once

#include "MemoryPool.h"
#ifdef WINDOWS
#define INITGUID  // Causes definition of SystemTraceControlGuid in evntrace.h.
#include <Wmistr.h>
#include <evntrace.h>
#endif

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ETW
{
	EVENT_TRACE_PROPERTIES *sessionProperties;
	EVENT_TRACE_LOGFILE logFile;
	TRACEHANDLE sessionHandle;
	TRACEHANDLE openedHandle;

	HANDLE processThreadHandle;

	bool isActive;

	static DWORD WINAPI RunProcessTraceThreadFunction(LPVOID parameter);
public:
	ETW();
	~ETW();

	enum Status
	{
		ETW_OK = 0,
		ETW_ERROR_ALREADY_EXISTS = 1,
		ETW_ERROR_ACCESS_DENIED = 2,
		ETW_FAILED = 3,
	};

	Status Start();
	bool Stop();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}