set(Windows_SRC
	CommonWindows.h
	ConcurrencyWindows.h
	ETWWindows.cpp
	EasyHook32.lib
	EasyHook64.lib
	HPTimerWindows.cpp
	HookWindows.cpp
	SocketWindows.h
	SymEngineWindows.cpp
	ThreadWindows.cpp
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
		Linux/ThreadLinux.cpp)
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
