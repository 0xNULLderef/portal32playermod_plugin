#ifndef UTILS_HPP
#define UTILS_HPP

#include <sdk.hpp>

// Platform specific - declare the __cdecl macro (different for windows) and the macro to complete the shared object names
#define __cdecl __attribute__((__cdecl__))

#define MODULE(name) name MODULE_EXTENSION
#define MODULE_EXTENSION ".so"

// Redeclare macro (not really redeclaring, just declaring something without the autism of actually doing so)
#define REDECL(name) decltype(name) name

// Detour stuff
#define DECL_DETOUR(name, ...) \
	using _##name = int(__rescall*)(void* thisptr, ##__VA_ARGS__); \
	static _##name name; \
	static int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DECL_DETOUR_T(type, name, ...) \
	using _##name = type(__cdecl*)(void* thisptr, ##__VA_ARGS__); \
	static _##name name; \
	static type __cdecl name##_Hook(void* thisptr, ##__VA_ARGS__);

#define DETOUR(name, ...) \
	int __rescall name##_Hook(void* thisptr, ##__VA_ARGS__)
#define DETOUR_T(type, name, ...) \
	type __cdecl name##_Hook(void* thisptr, ##__VA_ARGS__)

#define SAFE_DELETE(ptr) \
	if(ptr) { \
		delete ptr; \
		ptr = nullptr; \
	}

#endif // UTILS_HPP
