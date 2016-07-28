cmake_minimum_required (VERSION 2.8.11)
project (Brofiler)

IF (WIN32)
add_definitions(-DWIN32 -DWINDOWS)
ELSEIF(WIN64)
add_definitions(-DWIN64 -DWINDOWS)
ELSE()
add_definitions(-DLINUX64)
ENDIF()

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -g" )
add_subdirectory (ProfilerCore)
add_subdirectory (ProfilerTest)
