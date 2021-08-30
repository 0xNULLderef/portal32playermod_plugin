#include <vscript.hpp>

#include <console.hpp>
#include <offsets.hpp>
#include <server.hpp>

void printHelloWorld() {
	console->Print("Hello world\n");
}

const char* getPlayerName(int index) {
	return server->GetPlayerName(index);
}

REDECL(VScript::CreateVM);
DETOUR_T(IScriptVM*, VScript::CreateVM, ScriptLanguage_t language) {
	IScriptVM* g_pScriptVM = VScript::CreateVM(thisptr, language);
	vscript->g_pScriptVM = g_pScriptVM;
	ScriptRegisterFunction(g_pScriptVM, printHelloWorld, "Prints hello world to console.");
	ScriptRegisterFunction(g_pScriptVM, getPlayerName, "Gets player username by index (starting at 0)");
	return g_pScriptVM;
}

VScript::VScript() {}

bool VScript::Init() {
	this->scriptmanager = Interface::Create(vscript->Name(), "VScriptManager009");
	if(this->scriptmanager) {
		this->scriptmanager->Hook(VScript::CreateVM_Hook, VScript::CreateVM, Offsets::CreateVM);
		return true;
	}
	return false;
}

void VScript::Shutdown() {
	this->scriptmanager->Unhook(Offsets::CreateVM);
	Interface::Delete(this->scriptmanager);
}

VScript* vscript;
