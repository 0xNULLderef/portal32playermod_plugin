#ifndef SDL_HOOK_HPP
#define SDL_HOOK_HPP

#include <memory.hpp>

#define SDL_LIB_NAME "libSDL2-2.0.so.0"

template <typename T> class SDL_Hook {
public:
	SDL_Hook(const char* pszName);
	void Hook(T* hook);
	void Unhook();

	T original;
private:
	void** m_ppHook;
	void* m_pOriginal;
};

template <typename T> SDL_Hook<T>::SDL_Hook(const char* pszName) {
	// e8 8e 59 fe ff		CALL __i686.get_pc_thunk.ax
	// 05 xx xx xx xx		ADD EAX,xxxxxxxx
	// ff a0 xx xx xx xx	JMP dword ptr [EAX + xxxxxxxx]
	auto hLib = Memory::GetModuleHandleByName(SDL_LIB_NAME);
	auto uiSymbol = Memory::GetSymbolAddress<uintptr_t>(hLib, pszName);
	uintptr_t unAddOffset = *(uintptr_t*)(uiSymbol + 6);
	uintptr_t unJmpOffset = *(uintptr_t*)(uiSymbol + 12);
	// +5 is the offset coming from calling get_pc_thunk, it returns the RIP
	// which points to the ADD instruction, while our handle points to entry
	this->m_ppHook = (void**)(uiSymbol + unAddOffset + unJmpOffset + 5);
	this->m_pOriginal = *m_ppHook;
	this->original = (T)m_pOriginal;
}

template <typename T> void SDL_Hook<T>::Hook(T* hook) {
	*this->m_ppHook = (void*)hook;
}

template <typename T> void SDL_Hook<T>::Unhook() {
	*this->m_ppHook = this->m_pOriginal;
}

#endif // SDL_HOOK_HPP