#include "main.hpp"

DiscordUser cbConUser;
std::pair<int, char> cbDisconnected;
std::pair<int, char> cbError;
char cbJoinSecret;
char cbSpectateSecret;
DiscordUser cbJoinRequest;
bool cbConUserTriggerd, cbDisconnectedTriggerd, cbErrorTriggerd, cbJoinTriggerd,
    cbSpectateTriggerd, cbJoinRequestTriggerd;

static void rnlib::HandleDiscordReady(const DiscordUser *connectedUser) {
    cbConUser = *connectedUser;
    cbConUserTriggerd = true;
}

static void rnlib::HandleDiscordDisconnected(int errcode, const char *message) {
    cbDisconnected.first = errcode;
    cbDisconnected.second = *message;
    cbDisconnectedTriggerd = true;
}

static void rnlib::HandleDiscordError(int errcode, const char *message) {
    cbError.first = errcode;
    cbError.second = *message;
    cbConUserTriggerd = true;
}

static void rnlib::HandleDiscordJoin(const char *secret) {
    cbJoinSecret = *secret;
    cbJoinTriggerd = true;
}

static void rnlib::HandleDiscordSpectate(const char *secret) {
    cbSpectateSecret = *secret;
    cbSpectateTriggerd = true;
}

static void rnlib::HandleDiscordJoinRequest(const DiscordUser *request) {
    cbJoinRequest = *request;
    cbJoinRequestTriggerd = true;
}

GarrysMod::Lua::ILuaInterface* rnlib::ILuaServer;

LUA_FUNCTION(StartDiscordStatus) {
    const char *appid = LUA->GetString(1);

    DiscordEventHandlers handlers{};

    handlers.ready = HandleDiscordReady;
    handlers.disconnected = HandleDiscordDisconnected;
    handlers.errored = HandleDiscordError;
    handlers.joinGame = HandleDiscordJoin;
    handlers.spectateGame = HandleDiscordSpectate;
    handlers.joinRequest = HandleDiscordJoinRequest;

    Discord_Initialize(appid, &handlers, 1, 0);

    return 0;
}

LUA_FUNCTION(RunDiscordCallbacks) {
    Discord_RunCallbacks();
    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->GetField(-1, "hook");
    if (cbConUserTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordReady");
        LUA->PushString(cbConUser.userId);
        LUA->PushString(cbConUser.username);
        LUA->PushString(cbConUser.discriminator);
        LUA->PushString(cbConUser.avatar);
        LUA->PushString(cbConUser.username);
        LUA->Call(6, 0);
        cbConUserTriggerd = false;
    }

    if (cbDisconnectedTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordDisconnected");
        LUA->PushNumber(cbDisconnected.first);
        LUA->PushString(&cbDisconnected.second);
        LUA->Call(3, 0);
        cbDisconnectedTriggerd = false;
    }

    if (cbErrorTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordError");
        LUA->PushNumber(cbError.first);
        LUA->PushString(&cbError.second);
        LUA->Call(3, 0);
        cbErrorTriggerd = false;
    }

    if (cbJoinTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoin");
        LUA->PushString(&cbJoinSecret);
        LUA->Call(2, 0);
        cbJoinTriggerd = false;
    }

    if (cbSpectateTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordSpectate");
        LUA->PushString(&cbSpectateSecret);
        LUA->Call(2, 0);
        cbSpectateTriggerd = false;
    }

    if (cbJoinRequestTriggerd) {
        LUA->GetField(-1, "Run");
        LUA->PushString("DiscordJoinRequest");
        LUA->PushString(cbJoinRequest.userId);
        LUA->PushString(cbJoinRequest.username);
        LUA->PushString(cbJoinRequest.discriminator);
        LUA->PushString(cbJoinRequest.avatar);
        LUA->PushString(cbJoinRequest.username);
        LUA->Call(6, 0);
        cbJoinRequestTriggerd = false;
    }

    LUA->Pop(2);

    return 0;
}

LUA_FUNCTION(DiscordRespond) {
    const char *userid = LUA->GetString(1);
    const int reply = LUA->GetNumber(2);
    Discord_Respond(userid, reply);
    return 0;
}

LUA_FUNCTION(UpdateDiscordStatus) {
    DiscordRichPresence discordP;
    memset(&discordP, 0, sizeof(discordP));
    LUA->GetField(1, "state");
    discordP.state = LUA->GetString();
    LUA->GetField(1, "details");
    discordP.details = LUA->GetString();
    LUA->GetField(1, "startTimestamp");
    discordP.startTimestamp = LUA->GetNumber();
    LUA->GetField(1, "endTimestamp");
    discordP.endTimestamp = LUA->GetNumber();
    LUA->GetField(1, "largeImageKey");
    discordP.largeImageKey = LUA->GetString();
    LUA->GetField(1, "largeImageText");
    discordP.largeImageText = LUA->GetString();
    LUA->GetField(1, "smallImageKey");
    discordP.smallImageKey = LUA->GetString();
    LUA->GetField(1, "smallImageText");
    discordP.smallImageText = LUA->GetString();
    LUA->GetField(1, "partyId");
    discordP.partyId = LUA->GetString();
    LUA->GetField(1, "partySize");
    discordP.partySize = LUA->GetNumber();
    LUA->GetField(1, "partyMax");
    discordP.partyMax = LUA->GetNumber();
    LUA->GetField(1, "partyPrivacy");
    discordP.partyPrivacy = LUA->GetNumber();
    LUA->GetField(1, "matchSecret");
    discordP.matchSecret = LUA->GetString();
    LUA->GetField(1, "joinSecret");
    discordP.joinSecret = LUA->GetString();
    LUA->GetField(1, "spectateSecret");
    discordP.spectateSecret = LUA->GetString();
    LUA->GetField(1, "instance");
    discordP.instance = LUA->GetNumber();

    Discord_UpdatePresence(&discordP);
    return 0;
}

GMOD_MODULE_OPEN()
{
	rnlib::ILuaServer = reinterpret_cast<GarrysMod::Lua::ILuaInterface*>(LUA);

	rnlib::ILuaServer->CreateTable();
        PUSHFUNC(StartDiscordStatus)
        PUSHFUNC(RunDiscordCallbacks)
        PUSHFUNC(DiscordRespond)
        PUSHFUNC(UpdateDiscordStatus)
	rnlib::ILuaServer->SetField(GarrysMod::Lua::INDEX_GLOBAL, "rnlib");

	rnlib::ILuaServer->MsgColour(Color(236, 169, 81, 255), "[rnlib] Version '%s' loaded successfully.\n", _MODULE_VERSION_);

	return 0;
}

GMOD_MODULE_CLOSE()
{
    Discord_Shutdown();

	LUA->PushNil();
	LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "rnlib");
	return 0;
}