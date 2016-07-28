#pragma once

#ifndef WINDOWS
typedef void* HOOK_TRACE_INFO;
#else
#	include "easyhook.h"
#endif

#include "Common.h"
#include "MemoryPool.h"
#include "SymEngine.h"
#include "Event.h"
#include <vector>

namespace Profiler
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct HookDescription
{
	EventDescription* description{ nullptr };
	std::string nameString;
	std::string fileString;

	void Init(const Symbol& symbol);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*HookSlotFunction)();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Packed structure for assembler (4x8byte = 32 bytes)
struct HookData
{
	void* returnAddress;
	void* originalAddress;

	Profiler::EventData* eventData;
	Profiler::EventDescription* eventDescription;

	void Setup(Profiler::EventDescription* desc, void* address)
	{
		static_assert(sizeof(HookData) == 32, "HookData needs to compile to a size of 32 bytes");

		returnAddress = nullptr;
		originalAddress = address;
		eventDescription = desc;
		eventData = nullptr;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct HookSlotWrapper
{
	//! Pointer to the function to call
	void* functionAddress{ nullptr };

	//! Stores the actual hook
	HOOK_TRACE_INFO traceInfo{ nullptr };

	HookData* hookData;
	HookSlotFunction	hookFunction;

	bool IsEmpty() const { return functionAddress == nullptr; }

	bool Clear();
	bool Install(const Symbol& symbol, uint32_t threadID);
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hook
{
	Hook();
public:
	static Hook inst;

	static const uint SLOT_COUNT = 128;
	std::array<HookSlotWrapper, SLOT_COUNT> slots;
	std::vector<HookSlotWrapper*> availableSlots;
	MemoryPool<HookDescription> descriptions;
		
	bool Install(const Symbol& symbol, const std::vector<uint32_t>& threadIDs);
	bool Clear(const Symbol& symbol);
	bool ClearAll();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}