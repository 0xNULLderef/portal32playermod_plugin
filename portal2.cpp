#include <portal2.hpp>

#include <offsets.hpp>

// Portal2::Portal2() {}

Portal2::Portal2() {
	using namespace Offsets;

	// engine.so
	Dtor = 0;
	InternalSetValue = 19;
	InternalSetFloatValue = 20;
	InternalSetIntValue = 21;
	Create = 25;
	AutoCompletionFunc = 46;

	// libvstdlib.so
	RegisterConCommand = 9;
	UnregisterConCommand = 10;
	FindCommandBase = 13;
	InstallGlobalChangeCallback = 19;
	RemoveGlobalChangeCallback = 20;
	m_pConCommandList = 48;

	// server.so
	Think = 31;
	UTIL_PlayerByIndex = 70; // from CServerGameDLL::Think

	// vscript.so
	CreateVM = 8; // Technically not an offset but an index, but this is the way i'd manage it
}

Portal2* portal2;