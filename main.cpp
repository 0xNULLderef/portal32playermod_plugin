#include <main.hpp>

#include <iostream>
#include <sdk.hpp>
#include <portal2.hpp>
#include <console.hpp>
#include <interface.hpp>
#include <vscript.hpp>
#include <tier1.hpp>
#include <server.hpp>
#include <command.hpp>

Plugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Plugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin);

#include <stdio.h>

Plugin::Plugin() {
	Portal2* portal2 = new Portal2();
	(void)portal2; // Janky ass hack so i don't need to set -Wno-unused-variable
}

CON_COMMAND(p232_test, "p232_test (index) - find serverside offset\n") {
	void* entity = server->UTIL_PlayerByIndex(atoi(args[1]) + 1);
	if(entity != nullptr) {
		void* data = Memory::VMT<void* (*)(void*)>(entity, 87)(entity);
		
	} else {
		console->Print("name: nullptr\n");
	}
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	console = new Console();
	if(!console->Init()) return false;

	console->Print("(slowly) loading...\n");

	tier1 = new Tier1();
	if(!tier1->Init()) return false;

	vscript = new VScript();
	if(!vscript->Init()) return false;

	server = new Server();
	if(!server->Init()) return false;

	Command::RegisterAll();

	return true;
}

void Plugin::Unload() {
	console->Print("Gracefully returning the game to it's original state.\n");
	console->Shutdown();
	vscript->Shutdown();
	server->Shutdown();
	Command::UnregisterAll();
	tier1->Shutdown(); // Do this one later so that it doesn't try to unregister without tier1 loaded...
}

const char* Plugin::GetPluginDescription() {
	return "P2 32 player mod plugin (indev)";
}

// Unused callbacks
void Plugin::Pause() {}
void Plugin::UnPause() {}
void Plugin::LevelInit(char const* pMapName) {}
void Plugin::ServerActivate(void* pEdictList, int edictCount, int clientMax) {}
void Plugin::GameFrame(bool simulating) {}
void Plugin::LevelShutdown() {}
void Plugin::ClientFullyConnect(void* pEdict) {}
void Plugin::ClientActive(void* pEntity) {}
void Plugin::ClientDisconnect(void* pEntity) {}
void Plugin::ClientPutInServer(void* pEntity, char const* playername) {}
void Plugin::SetCommandClient(int index) {}
void Plugin::ClientSettingsChanged(void* pEdict) {}
int Plugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen) { return 0; }
int Plugin::ClientCommand(void* pEntity, const void*& args) { return 0; }
int Plugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) { return 0; }
void Plugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue) {}
void Plugin::OnEdictAllocated(void* edict) {}
void Plugin::OnEdictFreed(const void* edict) {}
