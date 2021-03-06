#include "../SymEngine.h"
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include "../Thread.h"

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void ReportLastError()
//{
//	LPVOID lpMsgBuf;
//	DWORD dw = GetLastError();
//
//	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//								NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
//								(LPTSTR)&lpMsgBuf, 0, NULL);
//
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK);
//	LocalFree(lpMsgBuf);
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SymEngine::SymEngine() : isInitialized(false), hProcess(CalculateCurrentThreadID()), needRestorePreviousSettings(false), previousOptions(0)
{
	static_assert(sizeof(hProcess) >= sizeof(HANDLE), "Too small hProcess type");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SymEngine::~SymEngine()
{
	Close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Symbol * const SymEngine::GetSymbol(DWORD64 dwAddress)
{
	if (dwAddress == 0)
		return nullptr;

	Symbol& symbol = cache[dwAddress];

	if (symbol.address != 0)
		return &symbol;

	static_assert(sizeof(void*) >= sizeof(DWORD64), "Downcast otherwise occurs here!");
	void *functionAddress[1] = { (void *)dwAddress };
	char **functionName = nullptr;

	functionName = backtrace_symbols(functionAddress, 1);

	if ( functionName != nullptr )
	{
		wchar_t widestringBuffer[1024];
		swprintf(widestringBuffer, 100, L"%hs", functionName[0]);
		symbol.address = dwAddress;
		symbol.function = widestringBuffer;
		free(functionName);
	}

	return &symbol;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// const char* USER_SYMBOL_SEARCH_PATH = "http://msdl.microsoft.com/download/symbols";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SymEngine::Init()
{
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SymEngine::Close()
{
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint SymEngine::GetCallstack(HANDLE hThread, CallStackBuffer& callstack)
{
	int nptrs = 0;
#define SIZE 512
	void *buffer[SIZE] = { nullptr };

	nptrs = backtrace(buffer, SIZE);
	printf("backtrace() returned %d addresses\n", nptrs);

	int index = 0;
	for ( ; index < nptrs; index++)
	{
		static_assert(sizeof(DWORD64) >= sizeof(void*), "Information lost may occur otherwise");
		DWORD64 dwAddress = (DWORD64)buffer[index];
		if (!dwAddress)
			break;

		if (index == callstack.size())
			return 0; // Too long callstack - possible error, let's skip it

		if (index > 0 && callstack[index - 1] == dwAddress)
			continue;

		callstack[index] = dwAddress;
	}

	return index;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
