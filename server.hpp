#pragma once

#include <interface.hpp>
#include <utils.hpp>
#include <sdk.hpp>

class Server {
public:
	Interface* g_ServerGameDLL = nullptr;

	using _GetAllServerClasses = ServerClass* (*)();
	_GetAllServerClasses GetAllServerClasses = nullptr;

public:
	Server();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("server"); }
};