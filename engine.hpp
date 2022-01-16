#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <interface.hpp>
#include <utils.hpp>

class Engine {
private:
	Interface* g_pServerPluginHandler;

public:
	Engine();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("engine"); }

public:
	DECL_DETOUR_T(void, LevelInit, const char* pMapName, const char* pMapEntities, const char* pOldLevel, const char* pLandmarkName, bool loadGame, bool background);
};

extern Engine* engine;

#endif // ENGINE_HPP
