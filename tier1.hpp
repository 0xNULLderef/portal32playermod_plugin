#ifndef TIER1_HPP
#define TIER1_HPP

#include <interface.hpp>
#include <sdk.hpp>
#include <utils.hpp>

#define TIER1 "libvstdlib"

class Tier1 {
public:
	Interface* g_pCVar = nullptr;

	_RegisterConCommand RegisterConCommand = nullptr;
	_UnregisterConCommand UnrgeisterConCommand = nullptr;
	_FindCommandBase FindCommandBase = nullptr;
	_InstallGlobalChangeCallback InstallGlobalChangeCallback = nullptr;
	_RemoveGlobalChangeCallback RemoveGlobalChangeCallback = nullptr;

	ConCommandBase* m_pConCommandList = nullptr;

	void* ConCommand_VTable = nullptr;
	void* ConVar_VTable = nullptr;
	void* ConVar_VTable2 = nullptr;
	_AutoCompletionFunc AutoCompletionFunc = nullptr;

	using _Dtor = int(__rescall*)(ConVar* thisptr);
	using _Create = int(__rescall*)(ConVar* thisptr, const char* pName, const char* pDefaultValue, int flags, const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

	_Dtor Dtor = nullptr;
	_Create Create = nullptr;

public:
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE(TIER1); }
};

extern Tier1* tier1;

#endif // TIER1_HPP
