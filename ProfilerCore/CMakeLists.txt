set(Windows_SRC
	Windows/CommonWindows.h
	Windows/ConcurrencyWindows.h
	Windows/ETWWindows.cpp
	Windows/EasyHook32.lib
	Windows/EasyHook64.lib
	Windows/HPTimerWindows.cpp
	Windows/HookWindows.cpp
	Windows/SocketWindows.h
	Windows/SymEngineWindows.cpp
	Windows/ThreadWindows.cpp
)
if (WIN32)
	set(Platform_SRC ${Windows_SRC} HookFunctions32.asm)
elseif(WIN64)
	set(Platform_SRC ${Windows_SRC} HookFunctions64.asm)
else()
	set(Platform_SRC Linux/CommonLinux.h
		Linux/ConcurrencyLinux.h
		Linux/ETWLinux.cpp
		Linux/HookLinux.cpp
		Linux/HPTimerLinux.cpp
		Linux/SocketLinux.h
		Linux/SymEngineLinux.cpp
		Linux/ThreadLinux.cpp
		${Windows_SRC})
	set_source_files_properties(${Windows_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

set(Common_SRC
	Brofiler.h
	Common.h
	Concurrency.h
	Core.cpp
	Core.h
	ETW.h
	Event.cpp
	Event.h
	EventDescriptionBoard.cpp
	EventDescriptionBoard.h
	Frame.hpp
	HPTimer.h
	Hook.h
	HookFunction.h
	MemoryPool.h
	Message.cpp
	Message.h
	ProfilerServer.cpp
	ProfilerServer.h
	Sampler.cpp
	Sampler.h
	Serialization.cpp
	Serialization.h
	Socket.h
	SymEngine.h
	Thread.h
	Thread.cpp
	Wsa.h
	easyhook.h
)

add_library (ProfilerCore ${Common_SRC} ${Platform_SRC})
target_include_directories (ProfilerCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
