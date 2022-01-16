#ifndef VSCRIPT_HPP
#define VSCRIPT_HPP

#include <sdk.hpp>
#include <interface.hpp>
#include <utils.hpp>

class VScript {
public:
	Interface* scriptmanager;
	void* g_pScriptVM = nullptr;

	using _Run = ScriptStatus_t (__cdecl*)(void* thisptr, const char* pszScript, bool bWait);
	_Run Run = nullptr;

public:
	VScript();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("vscript"); }

	DECL_DETOUR_T(IScriptVM*, CreateVM, ScriptLanguage_t language);

public:
	void DoChatCallbacks(int id, char* message);
	bool hasToResetVM = true;
};

extern VScript* vscript;

#endif // VSCRIPT_HPP
