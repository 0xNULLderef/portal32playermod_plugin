#include <sdk.hpp>

#include <utils.hpp>
#include <interface.hpp>

class Client {
private:
	Interface* g_ClientDLL = nullptr;
	Interface* g_HudChat = nullptr;

public:
	using _ChatPrintf = void (*)(void* thisptr, int iPlayerIndex, int iFilter, const char* fmt, ...);

	_ChatPrintf ChatPrintf = nullptr;

public:
	Client();
	bool Init();
	void Shutdown();
	const char* Name() { return MODULE("client"); }

public:
	DECL_DETOUR(MsgFunc_SayText2, bf_read &msg);
};

extern Client* client;