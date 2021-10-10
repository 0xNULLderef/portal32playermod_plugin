#include <sdk.hpp>
#include <utils.hpp>

class Engine {
public:
	using _GetPlayerNetworkIDString = const char* (*)(void* thisptr, const edict_t *pPlayerEdict);
	_GetPlayerNetworkIDString GetPlayerNetworkIDString = nullptr;
public:
	Engine();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("engine"); }
};

extern Engine* engine;