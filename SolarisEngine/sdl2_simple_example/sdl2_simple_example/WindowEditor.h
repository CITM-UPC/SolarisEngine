#pragma once
#include <string>
#include "App.h"
#include <imgui_impl_sdl2.h>



struct  SDL_Window;


class WindowEditor
{
	SDL_Window* _window = nullptr;
	void* _ctx = nullptr;
public:
	WindowEditor();
	~WindowEditor();
	void Create();
	void Shutdown();

	void HandleSDLEvent(SDL_Event& e);
	
	bool PumpEvents();

	void GetSize(int& w, int& h);

	SDL_Window* GetSDLWindow() { return _window; }
	void* GetGLContext() { return _ctx; }
	void BeginRender();
	void Render();
	void EndRender();


private:
	//SDL_Window* mWindow;
	////SDL_GLContext* mGLContext;
	//void* mGLContext;
	////WindowEditor mWindowEditor;
};