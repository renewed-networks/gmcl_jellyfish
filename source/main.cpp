#include "main.hpp"
#include "stdafx.hpp"
GarrysMod::Lua::ILuaInterface* JellyFish::ILuaServer;

LUA_FUNCTION_STATIC(version)
{
	LUA->PushString(_MODULE_VERSION_);
	return 1;
}

LUA_FUNCTION_STATIC(exist)
{
	LUA->CheckString(1);
	const char* path = LUA->GetString(1);
	std::string externalPath = std::string("jellyfish/") + path;
	if (std::filesystem::exists( externalPath ))
		LUA->PushNumber(1);
	else
		LUA->PushNumber(0);

	return 1;
}

LUA_FUNCTION_STATIC(write)
{
	LUA->CheckString(1);
	LUA->CheckString(2);
	const char* path = LUA->GetString(1);
	const char* content = LUA->GetString(2);
	std::string externalPath = std::string("jellyfish/") + path;
	std::string externalContent = content;

	std::ofstream ofs( externalPath );
    	ofs << externalContent;
    ofs.close();

	if (std::filesystem::exists( externalPath ))
		LUA->PushNumber(1);
	else
		LUA->PushNumber(0);

	return 1;
}

LUA_FUNCTION_STATIC(remove)
{
	LUA->CheckString(1);
	const char* path = LUA->GetString(1);
	std::string externalPath = std::string("jellyfish/") + path;

	if (externalPath == std::string("jellyfish/version.txt")) {
		LUA->PushNumber(0);
		return 1;
	}

    if (std::filesystem::exists(externalPath)) {
		std::filesystem::remove(externalPath);
		LUA->PushNumber(1);
    } else {
        LUA->PushNumber(0);
    }

	return 1;
}

LUA_FUNCTION_STATIC(filesize)
{
	LUA->CheckString(1);
	const char* path = LUA->GetString(1);
	std::string externalPath = std::string("jellyfish/") + path;

	if (std::filesystem::exists(externalPath)) {
		double size = std::filesystem::file_size(externalPath);
		LUA->PushNumber(size);
    } else {
        LUA->PushNumber(0);
    }

	return 1;
}

void Easy_Messagebox(const char *str, const char *title, int flags)
{
	MessageBox(NULL, str, title, flags);
}

LUA_FUNCTION_STATIC(msg_box)
{
	LUA->CheckString(1);
	LUA->CheckString(2);
	LUA->CheckNumber(3);
	const char* msg = LUA->GetString(1);
	const char* title = LUA->GetString(2);
	int flag = LUA->GetNumber(3);

	if (flag == 0)
		Easy_Messagebox(msg, title, 0x00000002L);
	if (flag == 1)
		Easy_Messagebox(msg, title, 0x00000006L);
	if (flag == 2)
		Easy_Messagebox(msg, title, 0x00004000L);
	if (flag == 3)
		Easy_Messagebox(msg, title, 0x00000000L);
	if (flag == 4)
		Easy_Messagebox(msg, title, 0x00000001L);
	if (flag == 5)
		Easy_Messagebox(msg, title, 0x00000005L);
	if (flag == 6)
		Easy_Messagebox(msg, title, 0x00000004L);
	if (flag == 7)
		Easy_Messagebox(msg, title, 0x00000003L);

	return 1;
}

GMOD_MODULE_OPEN()
{
	JellyFish::ILuaServer = reinterpret_cast<GarrysMod::Lua::ILuaInterface*>(LUA);

	JellyFish::ILuaServer->CreateTable();
		PUSHFUNC(version);
		PUSHFUNC(exist);
		PUSHFUNC(write);
		PUSHFUNC(remove);
		PUSHFUNC(filesize);
		PUSHFUNC(msg_box);
	JellyFish::ILuaServer->SetField(GarrysMod::Lua::INDEX_GLOBAL, "jellyfish");

	std::filesystem::create_directory( std::string("jellyfish") );

	std::ofstream ofs( std::string("jellyfish/version.txt") );
    ofs << _MODULE_VERSION_; 
    ofs.close();

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