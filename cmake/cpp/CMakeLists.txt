cmake_minimum_required(VERSION 3.10)
project(gmod-discord-rpc-buttons)
set(CMAKE_CXX_STANDARD 14)

if(UNIX)
	if(64BIT)
        set(CMAKE_CXX_FLAGS "-Os -fPIC")
    else()
    	set(CMAKE_CXX_FLAGS "-m32 -Os -s")
    endif()
endif()


set(BUILD_EXAMPLES OFF CACHE BOOL "Build example apps")
set(USE_STATIC_CRT ON CACHE BOOL "Use /MT[d] for dynamic library")
add_subdirectory(discord-rpc-buttons)
include_directories(${PROJECT_SOURCE_DIR}/discord-rpc-buttons/include)

add_subdirectory(gmod-module-base)

add_library(jellyfish_rpc SHARED main.cpp)

target_link_libraries(jellyfish_rpc discord-rpc-buttons)
target_link_libraries(jellyfish_rpc gmod-module-base)

SET_TARGET_PROPERTIES(jellyfish_rpc PROPERTIES PREFIX "gmcl_")
if(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		SET_TARGET_PROPERTIES(jellyfish_rpc PROPERTIES SUFFIX "_win32.dll")
	else()
		SET_TARGET_PROPERTIES(jellyfish_rpc PROPERTIES SUFFIX "_win64.dll")
	endif()
endif()