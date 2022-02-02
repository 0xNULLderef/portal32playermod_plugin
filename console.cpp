#include <console.hpp>

#include <memory.hpp>

bool Console::Init() {
	auto tier0 = Memory::GetModuleHandleByName(this->Name());
	if(tier0) {
		this->Msg = Memory::GetSymbolAddress<_Msg>(tier0, MSG_SYMBOL);
		this->ColorMsg = Memory::GetSymbolAddress<_ColorMsg>(tier0, CONCOLORMSG_SYMBOL);
		this->Warning = Memory::GetSymbolAddress<_Warning>(tier0, WARNING_SYMBOL);
		this->DevMsg = Memory::GetSymbolAddress<_DevMsg>(tier0, DEVMSG_SYMBOL);
		this->DevWarning = Memory::GetSymbolAddress<_DevWarning>(tier0, DEVWARNINGMSG_SYMBOL);

		this->LoggingSystem_PushLoggingState = Memory::GetSymbolAddress<_LoggingSystem_PushLoggingState>(tier0, "LoggingSystem_PushLoggingState");
		this->LoggingSystem_PopLoggingState = Memory::GetSymbolAddress<_LoggingSystem_PopLoggingState>(tier0, "LoggingSystem_PopLoggingState");
		this->LoggingSystem_RegisterLoggingListener = Memory::GetSymbolAddress<_LoggingSystem_RegisterLoggingListener>(tier0, "LoggingSystem_RegisterLoggingListener");
		this->LoggingSystem_HasTag = Memory::GetSymbolAddress<_LoggingSystem_HasTag>(tier0, "LoggingSystem_HasTag");
		this->LoggingSystem_SetChannelSpewLevelByTag = Memory::GetSymbolAddress<_LoggingSystem_SetChannelSpewLevelByTag>(tier0, "LoggingSystem_SetChannelSpewLevelByTag");

		Memory::CloseModuleHandle(tier0);
	}

	return this->Msg && this->ColorMsg && this->Warning && this->DevMsg && this->DevWarning;
}

void Console::Shutdown() {}

void Console::DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		console->Print("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			console->Print(" ");
			if ((i+1) % 16 == 0) {
				console->Print("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					console->Print(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					console->Print("   ");
				}
				console->Print("|  %s \n", ascii);
			}
		}
	}
}

ConsoleListener::ConsoleListener(std::function<void (const char*)> cbk) : cbk(cbk) {
	console->LoggingSystem_PushLoggingState(false, false);
	console->LoggingSystem_RegisterLoggingListener(this);
	console->LoggingSystem_SetChannelSpewLevelByTag("Console", LSEV_MESSAGE);
}

void ConsoleListener::Log(const LoggingContext* ctx, const char* msg) {
	if(!console->LoggingSystem_HasTag(ctx->channelID, "Console")) return;
	this->cbk(msg);
}

ConsoleListener::~ConsoleListener() {
	console->LoggingSystem_PopLoggingState(false);
}

Console* console;
