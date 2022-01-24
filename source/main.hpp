#ifndef RNLIB_MAIN_H
#define RNLIB_MAIN_H

#ifdef _WIN32
	#pragma once
#endif

#define _MODULE_VERSION_ "0.1"

#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/Lua/LuaInterface.h>
#include <GarrysMod/FunctionPointers.hpp>
#include <GarrysMod/InterfacePointers.hpp>
#include <discord_rpc.h>
#include <stdio.h>
#include <string.h>
#include <utility>

#define PUSHFUNC(name) \
LUA->PushCFunction(name); \
LUA->SetField(-2, #name);

namespace rnlib {
	extern GarrysMod::Lua::ILuaInterface* ILuaServer;

	HandleDiscordReady(const DiscordUser *connectedUser)
	HandleDiscordDisconnected(int errcode, const char *message)
	HandleDiscordError(int errcode, const char *message)
	HandleDiscordJoin(const char *secret)
	HandleDiscordSpectate(const char *secret)
	HandleDiscordJoinRequest(const DiscordUser *request)
}

#endif