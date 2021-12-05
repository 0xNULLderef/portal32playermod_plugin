#include <vscript.hpp>

#include <console.hpp>
#include <offsets.hpp>
#include <server.hpp>

#include <cstdio>
#include <unordered_set>
#include <string>

std::unordered_set<std::string> chatCallbackNames;

const char* GetPlayerName(int index) {
	return server->GetPlayerName(index);
}

void AddChatCallback(const char* funcName) {
	chatCallbackNames.insert(std::string(funcName));
}

REDECL(VScript::CreateVM);
DETOUR_T(IScriptVM*, VScript::CreateVM, ScriptLanguage_t language) {
	IScriptVM* g_pScriptVM = VScript::CreateVM(thisptr, language);
	auto vmInterface = Interface::Create(g_pScriptVM);
	if(vscript->hasToResetVM) {
		vscript->g_pScriptVM = g_pScriptVM;
		vscript->hasToResetVM = false;
	}
	vscript->Run = vmInterface->Original<_Run>(Offsets::Run);
	ScriptRegisterFunction(g_pScriptVM, GetPlayerName, "Gets player username by index");
	ScriptRegisterFunction(g_pScriptVM, AddChatCallback, "Adds chat callback called with player id and message");
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

void VScript::DoChatCallbacks(int id, char* message) {
	char buf[256];
	for(auto& callbackName: chatCallbackNames) {
		snprintf(buf, 256, "%s(%d, \"%s\")", callbackName.c_str(), id, message);
		vscript->Run(vscript->g_pScriptVM, buf, true);
	}
}

VScript* vscript;
