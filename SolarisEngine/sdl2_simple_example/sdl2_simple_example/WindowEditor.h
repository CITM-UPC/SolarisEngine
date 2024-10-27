#pragma once
#include <string>
#include "App.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>


struct  SDL_Window;


class WindowEditor
{
public:
    WindowEditor();
    ~WindowEditor();

    // 创建和销毁窗口
    void Create();
    void Shutdown();

    // 事件处理
    void HandleSDLEvent(SDL_Event& e);
    bool PumpEvents();

    // 渲染控制
    void BeginRender();
    void Render();
    void EndRender();

    

    // 获取窗口信息
    void GetSize(int& w, int& h);
    SDL_Window* GetSDLWindow() { return _window; }
    void* GetGLContext() { return _ctx; }


private:
    void resizeViewport(int width, int height);
    SDL_Window* _window = nullptr;  // SDL 窗口
    void* _ctx = nullptr;           // OpenGL 上下文
    bool _shouldClose = false;      // 用于指示退出请求
};