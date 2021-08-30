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

Server* server;
