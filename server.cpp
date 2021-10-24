#include <server.hpp>

#include <offsets.hpp>

#include <utils.hpp>

#include <dumphex.hpp>

Server::Server() {}

bool Server::Init() {
	this->g_ServerGameDLL = Interface::Create(this->Name(), "ServerGameDLL005");
	if(this->g_ServerGameDLL) {
		auto Think = this->g_ServerGameDLL->Original(Offsets::Think);
		Memory::Read<_UTIL_PlayerByIndex>(Think + Offsets::UTIL_PlayerByIndex, &this->UTIL_PlayerByIndex);
	}
	return this->g_ServerGameDLL && this->UTIL_PlayerByIndex;
}

void Server::Shutdown() {
	this->UTIL_PlayerByIndex = nullptr;
	Interface::Delete(this->g_ServerGameDLL);
}

const char* Server::GetPlayerName(int index) {
	void* entity = server->UTIL_PlayerByIndex(index + 1); // WHY DOES THE INDEX START AT 1?! VOLVO PLEASE FIX!
	if(entity != nullptr) {
		return (const char*)((uintptr_t)entity + Offsets::m_szNetname);
	} else {
		return "";
	}
}

edict_t* Server::GetPlayerEdict(int index) {
	void* player = server->UTIL_PlayerByIndex(index + 1);
	if(player) {
		edict_t* edict = *(edict_t**)((uintptr_t)player + 0x24);
		return edict;
	}
	return nullptr;
}

bool Server::IsPlayer(void* entity) {
	return Memory::VMT<bool (*)(void*)>(entity, Offsets::IsPlayer)(entity);
}

Server* server;
