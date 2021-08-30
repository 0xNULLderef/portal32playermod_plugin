#include <command.hpp>

#include <utils.hpp>
#include <tier1.hpp>
#include <cstdio>

std::vector<Command*>& Command::GetList() {
	static std::vector<Command*> list;
	return list;
}

Command::Command() :
	ptr(nullptr),
	version(0),
	isRegistered(false),
	isReference(false)
{}

Command::~Command() {
	if(!this->isReference) SAFE_DELETE(this->ptr);
}

ConCommand* Command::ThisPtr() {
	return this->ptr;
}

Command::Command(const char* pName, _CommandCallback callback, const char* pHelpString, int flags, _CommandCompletionCallback completionFunc) :
	isReference(false),
	isRegistered(false),
	version(0)
{
	this->ptr = new ConCommand(pName, callback, pHelpString, flags, completionFunc);
	Command::GetList().push_back(this);
}

void Command::Register() {
	if(!this->isRegistered) {
		this->ptr->ConCommandBase_VTable = tier1->ConCommand_VTable;
		tier1->RegisterConCommand(tier1->g_pCVar->ThisPtr(), this->ptr);
		tier1->m_pConCommandList = this->ptr;
	}
	this->isRegistered = true;
}

void Command::Unregister() {
	if(this->isRegistered) {
		tier1->UnrgeisterConCommand(tier1->g_pCVar->ThisPtr(), this->ptr);
	}
	this->isRegistered = false;
}

bool Command::operator!() {
	return this->ptr == nullptr;
}

int Command::RegisterAll() {
	auto result = 0;
	for(const auto& command : Command::GetList()) {
		command->Register();
		++result;
	}
	return result;
}

void Command::UnregisterAll() {
	for(const auto& command : Command::GetList()) {
		command->Unregister();
	}
}
