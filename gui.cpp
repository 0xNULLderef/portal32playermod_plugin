#include <gui.hpp>

#include <memory.hpp>
#include <dumphex.hpp>
#include <sdl_hook.hpp>

#include <SDL2/SDL.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

void SwapWindow(SDL_Window* window) {
    ImGui_ImplSDL2_InitForOpenGL(window, nullptr);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);

	ImGui::NewFrame();

	const auto& displaySize = ImGui::GetIO().DisplaySize;
	if(displaySize.x > 0.0f && displaySize.y > 0.0f) {
		if(ImGui::IsKeyPressed(SDL_SCANCODE_INSERT)) {
			gui->open = !gui->open;
		}
		if(gui->open) {
			ImGui::Begin("Portal2Util", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			ImGui::Checkbox("It works?", &gui->open);
			ImGui::End();
		}
	}

	ImGui::EndFrame();	
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	gui->m_SwapWindowHook->original(window);
}

int PollEvent(SDL_Event* event) {
	auto result = gui->m_PollEventHook->original(event);
	ImGui_ImplSDL2_ProcessEvent(event);
	if(result && gui->open) event->type = 0;

	return result;
}

GUI::GUI() {}

bool GUI::Init() {
	this->m_SwapWindowHook = new SDL_Hook<_SwapWindow>("SDL_GL_SwapWindow");
	this->m_PollEventHook = new SDL_Hook<_PollEvent>("SDL_PollEvent");

	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();


	this->m_SwapWindowHook->Hook((_SwapWindow*)&SwapWindow);
	this->m_PollEventHook->Hook((_PollEvent*)&PollEvent);

	return true;
}

void GUI::Shutdown() {
	m_SwapWindowHook->Unhook();
	m_PollEventHook->Unhook();
}

GUI* gui;