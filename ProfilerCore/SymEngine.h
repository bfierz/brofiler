#pragma once
#include "Common.h"
#include <string>
#include <unordered_map>
#include <array>
#include <vector>

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Symbol
{
	DWORD64 address{ 0 };
	DWORD64 offset{ 0 };
	std::wstring module;
	std::wstring file;
	std::wstring function;
	uint32_t line{ 0 };
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::array<DWORD64, 512> CallStackBuffer;
typedef std::vector<DWORD64> CallStack;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::unordered_map<DWORD64, Symbol> SymbolCache;
class SymEngine
{
	HANDLE hProcess;
	SymbolCache cache;

	bool isInitialized{ false };

	bool needRestorePreviousSettings{ false };
	DWORD previousOptions{ 0 };
	static const size_t MAX_SEARCH_PATH_LENGTH = 2048;
	char previousSearchPath[MAX_SEARCH_PATH_LENGTH];
public:
	SymEngine();
	~SymEngine();

	void Init();
	void Close();

	// Get Symbol from PDB file
	const Symbol * const GetSymbol(DWORD64 dwAddress);

	// Collects Callstack
	uint GetCallstack(HANDLE hThread, CallStackBuffer& callstack);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
