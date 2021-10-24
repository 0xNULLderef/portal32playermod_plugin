#include <main.hpp>

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
#include <dumphex.hpp>

#include <iostream>

Plugin plugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(Plugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin);

Plugin::Plugin() {
	Portal2* portal2 = new Portal2();
	(void)portal2; // Janky ass hack so i don't need to set -Wno-unused-variable
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

CON_COMMAND(ut_meme, "the meme, absolutely based.") {
	auto vengineClient = Interface::Create("engine.so", "VEngineClient015");
	uintptr_t SetTimescale = vengineClient->Original(158);
	Interface::Delete(vengineClient);
	uint8_t* ptr1 = (uint8_t*)(SetTimescale + 12);
	uint8_t backup[8];
	memcpy(backup, ptr1, 8);
	Memory::UnProtect((void*)ptr1, 8);

	// 8b 83 7c fe ff ff	MOV		EAX,dword ptr [EBX + 0xfffffe7c]
	// 5b					POP		EBX
	// c3					RET
	ptr1[0] = 0x8b;
	ptr1[1] = 0x83;
	ptr1[2] = 0x7c;
	ptr1[3] = 0xfe;
	ptr1[4] = 0xff;
	ptr1[5] = 0xff;
	ptr1[6] = 0x5b;
	ptr1[7] = 0xc3;

	using _DumpSv = uintptr_t(__cdecl *)();
	_DumpSv DumpSv = (_DumpSv)SetTimescale;
	uintptr_t sv = DumpSv();

	// copy the backup back
	memcpy(ptr1, backup, 8);

	console->Print("sv: %x\n", *(void**)sv);
	console->Print("=============================\n");
	DumpHex((void*)sv, 0x200);
	console->Print("=============================\n");
	DumpHex(*(void**)sv, 0x200);
	console->Print("=============================\n");
}

bool Plugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory) {
	console = new Console();
	if(!console->Init()) return false;

	console->Print("(slowly) loading...\n");

	tier1 = new Tier1();
	if(!tier1->Init()) return false;

	

	server = new Server();
	if(!server->Init()) return false;

	engine = new Engine();
	if(!engine->Init()) return false;
	
	vscript = new VScript();
	if(!vscript->Init()) return false;

	Command::RegisterAll();

	return true;
}

void Plugin::Unload() {
	console->Print("Gracefully returning the game to it's original state.\n");
	console->Shutdown();
	vscript->Shutdown();
	engine->Shutdown();
	server->Shutdown();
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
