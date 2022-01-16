#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sdk.hpp>
#include <vector>

class Command {
private:
	ConCommand* ptr;

public:
	int version;
	bool isRegistered;
	bool isReference;

public:
	static std::vector<Command*>& GetList();

public:
	Command();
	~Command();
	Command(const char* name);
	Command(const char* pName, _CommandCallback callback, const char* pHelpString, int flags = 0, _CommandCompletionCallback completionFunc = nullptr);

	ConCommand* ThisPtr();

	void Register();
	void Unregister();

	bool operator!();

	static int RegisterAll();
	static void UnregisterAll();
};

#define CON_COMMAND(name, description) \
	void name##_callback(const CCommand& args); \
	Command name = Command(#name, name##_callback, description); \
	void name##_callback(const CCommand& args)

#endif // COMMAND_HPP
