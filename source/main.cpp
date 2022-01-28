#include "main.hpp"
GarrysMod::Lua::ILuaInterface* JellyFish::ILuaServer;

LUA_FUNCTION_STATIC(version)
{
	LUA->PushString(_MODULE_VERSION_);
	return 1;
}

GMOD_MODULE_OPEN()
{
	JellyFish::ILuaServer = reinterpret_cast<GarrysMod::Lua::ILuaInterface*>(LUA);

	JellyFish::ILuaServer->CreateTable();
		PUSHFUNC(version)
	JellyFish::ILuaServer->SetField(GarrysMod::Lua::INDEX_GLOBAL, "jellyfish");

	JellyFish::ILuaServer->MsgColour(Color(236, 169, 81, 255), "[JellyFish] Version '%s' loaded successfully.\n", _MODULE_VERSION_);
	return 0;
}

GMOD_MODULE_CLOSE()
{
	LUA->PushNil();
	LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "jellyfish");
	JellyFish::ILuaServer = nullptr;
	return 0;
}