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
	LevelInit = 3;

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
	m_szNetname = 3901;

	// vscript.so
	CreateVM = 8;
	Run = 8; // IScriptVM

	// client.so
	MsgFunc_SayText2 = 35;
	GetHud = 146;
	FindElement = 161;
	ChatPrintf = 25;
}

Portal2* portal2;
