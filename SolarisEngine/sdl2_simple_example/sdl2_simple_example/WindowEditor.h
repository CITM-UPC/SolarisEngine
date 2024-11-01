#pragma once

#include <string>
#include "App.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "WindowImGui.h" // Incluir la clase WindowImGui

struct SDL_Window;

class WindowEditor {
public:
    WindowEditor();
    ~WindowEditor();

    void Create();
    void Shutdown();
    void HandleSDLEvent(SDL_Event& e);
    bool PumpEvents(SDL_Event& event);
    void BeginRender();
    void Render();
    void EndRender();

    WindowImGui* GetImGuiWindow();
    ImGuiIO* GetImGuiIO();

private:
    void resizeViewport(int width, int height);
    SDL_Window* _window = nullptr;  // SDL Window
    void* _ctx = nullptr;           // OpenGL context
    bool _shouldClose = false;      // Flag to indicate if the window should close
    WindowImGui* _windowImGui;     // Instancia de WindowImGui
    ImGuiIO* g_io = nullptr;   
};
