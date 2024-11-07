#include "WindowEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <string>

WindowEditor::WindowEditor()
	: _windowImGui(nullptr) { // Inicializa el puntero de WindowImGui a nullptr
}

WindowEditor::~WindowEditor() {
	if (_window) {
		Shutdown();
	}
}

void WindowEditor::Create() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	g_io = &ImGui::GetIO();
	g_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard controls
	g_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable docking
	//g_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable multiple viewports
	/*Fin DOCKING*/


	_window = SDL_CreateWindow("SolarisEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app->WINDOW_SIZE.x, app->WINDOW_SIZE.y, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	_ctx = SDL_GL_CreateContext(_window);
	if (!_ctx) {
		SDL_DestroyWindow(_window); // Destroy the window
		throw std::exception(SDL_GetError());
	}
	SDL_SetWindowMinimumSize(_window, 200, 200);

	if (!_window) throw std::exception(SDL_GetError());
	SDL_SetWindowResizable(_window, SDL_TRUE);

	if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw std::exception(SDL_GetError());
	if (SDL_GL_SetSwapInterval(1) != 0) throw std::exception(SDL_GetError());



	ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
	ImGui_ImplOpenGL3_Init("#version 440");

	//frameBuffer = new FrameBuffer(app->WINDOW_SIZE.x, app->WINDOW_SIZE.y);


	_windowImGui = new WindowImGui(); // Inicializa WindowImGui
	_windowImGui->Create(); // Crea los paneles



}

void WindowEditor::Shutdown() {
	if (_windowImGui) {
		_windowImGui->Shutdown(); // Cierra los paneles de ImGui
		delete _windowImGui; // Libera memoria
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void WindowEditor::HandleSDLEvent(SDL_Event& e) {
	//ImGui_ImplSDL2_ProcessEvent(&e);
	if (_windowImGui) {
		_windowImGui->HandleSDLEvent(e); // Pasa el evento a WindowImGui
	}
}

void WindowEditor::BeginRender() {
	int width, height;
	SDL_GetWindowSize(_window, &width, &height);
	resizeViewport(width, height);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
}

void WindowEditor::Render() {
	BeginRender();


	if (_windowImGui) {
		_windowImGui->Render(); // Llama al m�todo de renderizado de WindowImGui
	}





	EndRender();
	SDL_GL_SwapWindow(_window);
}

bool WindowEditor::PumpEvents(SDL_Event& e) {

	HandleSDLEvent(e);
	if (e.type == SDL_QUIT) {
		return false; // Cierra el editor si se recibe un evento de salida
	}

	return true;
}

void WindowEditor::EndRender() {


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (g_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(_window, _ctx);
	}

}

WindowImGui* WindowEditor::GetImGuiWindow()
{
	
	return _windowImGui;
}

void WindowEditor::resizeViewport(int width, int height) {
	glViewport(0, 0, width, height);
	//frameBuffer->RescaleFrameBuffer(width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspectRatio = (float)width / (float)height;
	glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
ImGuiIO* WindowEditor::GetImGuiIO()
{
	return g_io;
}

FrameBuffer* WindowEditor::GetFrameBuffer()
{
	return frameBuffer;
}
