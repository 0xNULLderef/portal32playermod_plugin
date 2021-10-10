#include <engine.hpp>

#include <interface.hpp>
#include <offsets.hpp>

Engine::Engine() {}

bool Engine::Init() {
	if(auto g_VEngineServer = Interface::Create(this->Name(), "VEngineServer022", false)) {
		this->GetPlayerNetworkIDString = g_VEngineServer->Original<_GetPlayerNetworkIDString>(Offsets::GetPlayerNetworkIDString);
		Interface::Delete(g_VEngineServer);
	}
	return this->GetPlayerNetworkIDString;
}

void Engine::Shutdown() {
	this->GetPlayerNetworkIDString = nullptr;
}

Engine* engine;