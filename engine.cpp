#include <engine.hpp>

#include <offsets.hpp>
#include <vscript.hpp>

REDECL(Engine::LevelInit);

DETOUR_T(void, Engine::LevelInit, const char* pMapName, const char* pMapEntities, const char* pOldLevel, const char* pLandmarkName, bool loadGame, bool background) {
	vscript->hasToResetVM = true;
	Engine::LevelInit(thisptr, pMapName, pMapEntities, pOldLevel, pLandmarkName, loadGame, background);
}

Engine::Engine() {}

bool Engine::Init() {
	this->g_pServerPluginHandler = Interface::Create(this->Name(), "ISERVERPLUGINHELPERS001");
	if(this->g_pServerPluginHandler) {
		this->g_pServerPluginHandler->Hook(Engine::LevelInit_Hook, Engine::LevelInit, Offsets::LevelInit);
	}

	return this->g_pServerPluginHandler;
}

void Engine::Shutdown() {
	this->g_pServerPluginHandler->Unhook(Offsets::LevelInit);
	Interface::Delete(this->g_pServerPluginHandler);
}

Engine* engine;
