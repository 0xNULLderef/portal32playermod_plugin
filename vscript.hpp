#pragma once

#include <interface.hpp>
#include <utils.hpp>

class VScript {
public:
	Interface* scriptmanager;
	void* g_pScriptVM;

public:
	VScript();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("vscript"); }

	DECL_DETOUR_T(IScriptVM*, CreateVM, ScriptLanguage_t language);
};

extern VScript* vscript;