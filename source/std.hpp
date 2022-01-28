#ifndef JELLYFISH_EXTERNAL_H
#define JELLYFISH_EXTERNAL_H

#ifdef _WIN32
	#pragma once
#endif

#include "main.hpp"

namespace JellyFish::External {
	void post_token(string token);
    void OpenLib(GarrysMod::Lua::ILuaInterface* LUA);
	void Initialize(GarrysMod::Lua::ILuaInterface* LUA);
	void Deinitialize(GarrysMod::Lua::ILuaInterface* LUA);
}

#endif