#pragma once

#ifdef WINDOWS
#include <windows.h>
#endif

// MEMORY //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MALLOC_ALIGN(ALIGNMENT, SIZE) _aligned_malloc(ALIGNMENT, SIZE)
#define FREE_ALIGN(PTR) _aligned_free(PTR)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
