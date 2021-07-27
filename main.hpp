#pragma once

#include <sdk.hpp>

// Main plugin class
class Plugin : public IServerPluginCallbacks {
public:
	Plugin();

	// All default, required methods for a source server plugin
	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void Unload();
	virtual void Pause();
	virtual void UnPause();
	virtual const char* GetPluginDescription();
	virtual void LevelInit(char const* pMapName);
	virtual void ServerActivate(void* pEdictList, int edictCount, int clientMax);
	virtual void GameFrame(bool simulating);
	virtual void LevelShutdown();
	virtual void ClientFullyConnect(void* pEdict);
	virtual void ClientActive(void* pEntity);
	virtual void ClientDisconnect(void* pEntity);
	virtual void ClientPutInServer(void* pEntity, char const* playername);
	virtual void SetCommandClient(int index);
	virtual void ClientSettingsChanged(void* pEdict);
	virtual int ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen);
	virtual int ClientCommand(void* pEntity, const void*& args);
	virtual int NetworkIDValidated(const char* pszUserName, const char* pszNetworkID);
	virtual void OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue);
	virtual void OnEdictAllocated(void* edict);
	virtual void OnEdictFreed(const void* edict);
};

extern Plugin plugin;