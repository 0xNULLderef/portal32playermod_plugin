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
#include <engine.hpp>
#include <offsets.hpp>

Plugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Plugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin);

Plugin::Plugin() {
	Portal2* portal2 = new Portal2();
	(void)portal2; // Janky ass hack so i don't need to set -Wno-unused-variable
}

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for(i = 0; i < size; ++i) {
		console->Print("%02X ", ((unsigned char*)data)[i]);
		if(((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if((i+1) % 8 == 0 || i+1 == size) {
			console->Print(" ");
			if((i+1) % 16 == 0) {
				console->Print("|  %s \n", ascii);
			} else if(i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if((i+1) % 16 <= 8) {
					console->Print(" ");
				}
				for(j = (i+1) % 16; j < 16; ++j) {
					console->Print("   ");
				}
				console->Print("|  %s \n", ascii);
			}
		}
	}
}

CON_COMMAND(ut_mod, "modifies the game to use 32 player mod (livepatch)") {
	auto serverGameClients = Interface::Create("server.so", "ServerGameClients004");
	uintptr_t GetPlayerLimits = serverGameClients->Original(0);
	Interface::Delete(serverGameClients);
	uint8_t* ptr1 = (uint8_t*)(GetPlayerLimits + 0x46);
	Memory::UnProtect((void*)ptr1, 1);
	ptr1[0] = 0x20;
	auto serverGameDLL = Interface::Create("server.so", "ServerGameDLL005");
	uintptr_t ApplyGameSettings = serverGameDLL->Original(38);
	Interface::Delete(serverGameDLL);
	uint8_t* ptr2 = (uint8_t*)(ApplyGameSettings + 0x8b5);
	Memory::UnProtect((void*)ptr2, 1);
	ptr2[0] = 0x20;
	console->Print("Patching done!\n");
}

CON_COMMAND(ut_read, "modifies the game to use 32 player mod (livepatch)") {
	auto serverGameClients = Interface::Create("server.so", "ServerGameClients004");
	uintptr_t GetPlayerLimits = serverGameClients->Original(0);
	Interface::Delete(serverGameClients);
	uint8_t* ptr1 = (uint8_t*)(GetPlayerLimits + 0x46);
	auto serverGameDLL = Interface::Create("server.so", "ServerGameDLL005");
	uintptr_t ApplyGameSettings = serverGameDLL->Original(38);
	Interface::Delete(serverGameDLL);
	uint8_t* ptr2 = (uint8_t*)(ApplyGameSettings + 0x8b5);
	console->Print("Values: 0x%02x 0x%02x (should be 0x20 0x20)\n", ptr1[0], ptr2[0]);
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	console = new Console();
	if(!console->Init()) return false;

	console->Print("(slowly) loading...\n");

	tier1 = new Tier1();
	if(!tier1->Init()) return false;

	vscript = new VScript();
	if(!vscript->Init()) return false;

	engine = new Engine();
	if(!engine->Init()) return false;

	// server = new Server();
	// if(!server->Init()) return false;

	Command::RegisterAll();

	return true;
}

void Plugin::Unload() {
	console->Print("Gracefully returning the game to it's original state.\n");
	console->Shutdown();
	vscript->Shutdown();
	// server->Shutdown();
	Command::UnregisterAll();
	tier1->Shutdown(); // Do this one later so that it doesn't try to unregister without tier1 loaded...
}

const char* Plugin::GetPluginDescription() {
	return "ut (indev)";
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
