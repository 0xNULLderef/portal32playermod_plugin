#include <server.hpp>

#include <offsets.hpp>

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
	void* entity = server->UTIL_PlayerByIndex(index); // i still hate the indexes starting at 1
	if(entity != nullptr) {
		return (const char*)((uintptr_t)entity + Offsets::m_szNetname);
	} else {
		return "";
	}
}

Server* server;
