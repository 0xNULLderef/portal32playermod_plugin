#include <vscript.hpp>

#include <console.hpp>
#include <cstdio>

void printHelloWorld() {
	console->Print("Hello world\n");
}

REDECL(VScript::CreateVM);
DETOUR_T(IScriptVM*, VScript::CreateVM, ScriptLanguage_t language) {
	IScriptVM* g_pScriptVM = VScript::CreateVM(thisptr, language);
	vscript->g_pScriptVM = g_pScriptVM;
	ScriptRegisterFunction(g_pScriptVM, printHelloWorld, "Prints hello world to console.");
	return g_pScriptVM;
}

VScript::VScript() {}

bool VScript::Init() {
	this->scriptmanager = Interface::Create(vscript->Name(), "VScriptManager009");
	if(this->scriptmanager) {
		this->scriptmanager->Hook(VScript::CreateVM_Hook, VScript::CreateVM, 8);
		return true;
	}
	return false;
}

void VScript::Shutdown() {
	Interface::Delete(this->scriptmanager);
}

VScript* vscript;