#include "WindowEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "MyWindow.h"
#include <SDL2/SDL.h>
#include <string>



WindowEditor::WindowEditor()
{

}

WindowEditor::~WindowEditor()
{
    if (_window) {
        Shutdown();
    }
}

void WindowEditor::Create()
{


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    _window = SDL_CreateWindow("Solari", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
   _ctx = SDL_GL_CreateContext(_window);
if (!_ctx) {
    SDL_DestroyWindow(_window);  // 清理窗口
    throw std::exception(SDL_GetError());
}
    SDL_SetWindowMinimumSize(_window, 200, 200);
    _ctx = SDL_GL_CreateContext(_window);


  

    //gladLoadGLLoader(SDL_GL_GetProcAddress);



   
    
    if (!_window) throw std::exception(SDL_GetError());
    SDL_SetWindowResizable(_window, SDL_TRUE);
   
    
    if (SDL_GL_MakeCurrent(_window, _ctx) != 0) throw std::exception(SDL_GetError());
    if (SDL_GL_SetSwapInterval(1) != 0) throw std::exception(SDL_GetError());

    ImGui_ImplSDL2_InitForOpenGL(_window, _ctx);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void WindowEditor::Shutdown() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
void WindowEditor::HandleSDLEvent(SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
}

void WindowEditor::BeginRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void WindowEditor::Render()
{
    PumpEvents();
    BeginRender();
    ImGui::ShowDemoWindow();

    EndRender();

    SDL_GL_SwapWindow(_window);
}

bool WindowEditor::PumpEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        HandleSDLEvent(e);

        // 处理其他 SDL 事件
        if (e.type == SDL_QUIT) {
            // 处理退出事件
            return false;
        }
    }
    return true;
}



void WindowEditor::EndRender() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

