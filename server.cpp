#include <server.hpp>

#include <offsets.hpp>

Server::Server() {}

bool Server::Init() {
	this->g_ServerGameDLL = Interface::Create(this->Name(), "ServerGameDLL005");
	this->GetAllServerClasses = this->g_ServerGameDLL->Original<_GetAllServerClasses>(Offsets::GetAllServerClasses);

	return this->g_ServerGameDLL && this->GetAllServerClasses;
}