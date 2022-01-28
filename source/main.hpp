#ifndef JELLYFISH_MAIN_H
#define JELLYFISH_MAIN_H

#ifdef _WIN32
	#pragma once
#endif

#define _MODULE_VERSION_ "0.1.1"

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/LuaInterface.h>
#include <GarrysMod/FunctionPointers.hpp>
#include <GarrysMod/InterfacePointers.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <filesystem>

#define LUA_PUSHTYPEONCE(type, data) \
LUA->Push##type(data); \
return 1;

#define PUSHFUNC(name) \
LUA->PushCFunction(name); \
LUA->SetField(-2, #name);

namespace JellyFish {
	extern GarrysMod::Lua::ILuaInterface* ILuaServer;
}

#endif