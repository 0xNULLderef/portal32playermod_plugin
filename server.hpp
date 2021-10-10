#pragma once

#include <interface.hpp>
#include <utils.hpp>
#include <sdk.hpp>

class Server {
public:
	Interface* g_ServerGameDLL = nullptr;

	using _UTIL_PlayerByIndex = void*(__cdecl*)(int index);
	_UTIL_PlayerByIndex UTIL_PlayerByIndex = nullptr;

	CGlobalVars* gpGlobals = nullptr;

public:
	Server();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("server"); }

public:
	const char* GetPlayerName(int index);
	edict_t* GetPlayerEdict(int index);
	bool IsPlayer(void* entity);
};

extern Server* server;
