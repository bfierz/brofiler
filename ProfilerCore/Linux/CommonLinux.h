#pragma once

#include <cstdint>
#include <cstdlib>
#include <list>

// MEMORY //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MALLOC_ALIGN(SIZE, ALIGNMENT) aligned_alloc(ALIGNMENT, SIZE)
#define FREE_ALIGN(PTR) free(PTR)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TYPES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef uint32_t HANDLE;
typedef uint64_t DWORD64;
typedef uint64_t CONTEXT;
typedef void* LPVOID;
typedef uint64_t EVENT_TRACE_PROPERTIES;
typedef uint64_t EVENT_TRACE_LOGFILE;
typedef uint64_t TRACEHANDLE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////