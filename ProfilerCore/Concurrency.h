#pragma once

#ifdef LINUX64
#include "Linux/ConcurrencyLinux.h"
#else
#include "Windows/ConcurrencyWindows.h"
#endif

namespace Profiler
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CriticalSectionScope
{
	CriticalSection &section;

public:
	CriticalSectionScope( CriticalSection& _lock ) : section(_lock) 
	{
		section.Enter(); 
	}

	~CriticalSectionScope() 
	{ 
		section.Leave(); 
	}

	CriticalSectionScope(const CriticalSectionScope&) = delete;
	CriticalSectionScope& operator=(const CriticalSectionScope&) = delete;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CRITICAL_SECTION(criticalSection) CriticalSectionScope generatedCriticalSectionScope##__LINE__(criticalSection); 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}