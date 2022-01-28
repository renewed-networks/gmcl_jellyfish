#include "main.hpp"
#include "std.hpp"
GarrysMod::Lua::ILuaInterface* JellyFish::ILuaServer;

std::vector<std::string> split(const std::string& str, const std::string& delim) {
	std::vector<std::string> keys;
	size_t prev = 0, pos = 0;
	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos)
			pos = str.length();

		std::string key = str.substr(prev, pos - prev);
		if (!key.empty())
			keys.push_back(key);
		prev = pos + delim.length();

	} while (pos < str.length() && prev < str.length());

	return keys;
}

std::string exec(const char* cmd){
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

LUA_FUNCTION_STATIC(version)
{
	LUA->PushString(_MODULE_VERSION_);
	return 1;
}

LUA_FUNCTION_STATIC(exist)
{
	if (std::filesystem::exists( std::string("jellyfish") ))
		LUA->PushNumber(1);
	else
		LUA->PushNumber(0);

	return 1;
}

GMOD_MODULE_OPEN()
{
	JellyFish::ILuaServer = reinterpret_cast<GarrysMod::Lua::ILuaInterface*>(LUA);

	JellyFish::ILuaServer->CreateTable();
		PUSHFUNC(version)
		PUSHFUNC(exist);
	JellyFish::ILuaServer->SetField(GarrysMod::Lua::INDEX_GLOBAL, "jellyfish");

	std::filesystem::create_directory( std::string("jellyfish") );

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