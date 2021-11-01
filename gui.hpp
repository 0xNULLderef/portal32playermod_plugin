#include <sdk.hpp>
#include <utils.hpp>
#include <interface.hpp>
#include <sdl_hook.hpp>

#include <SDL2/SDL.h>

class GUI {
public:
	using _SwapWindow = void (*)(SDL_Window* window);
	using _PollEvent = int (*)(SDL_Event* event);;

	SDL_Hook<_SwapWindow>* m_SwapWindowHook;
	SDL_Hook<_PollEvent>* m_PollEventHook;

public:
	bool open = false;

public:
	GUI();
	bool Init();
	void Shutdown();
	const char* Name() { return "libSDL2-2.0.so.0"; }
};

extern GUI* gui;