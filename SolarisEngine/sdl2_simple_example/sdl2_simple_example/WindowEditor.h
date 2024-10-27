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

    // ���������ٴ���
    void Create();
    void Shutdown();

    // �¼�����
    void HandleSDLEvent(SDL_Event& e);
    bool PumpEvents();

    // ��Ⱦ����
    void BeginRender();
    void Render();
    void EndRender();

    

    // ��ȡ������Ϣ
    void GetSize(int& w, int& h);
    SDL_Window* GetSDLWindow() { return _window; }
    void* GetGLContext() { return _ctx; }


private:
    void resizeViewport(int width, int height);
    SDL_Window* _window = nullptr;  // SDL ����
    void* _ctx = nullptr;           // OpenGL ������
    bool _shouldClose = false;      // ����ָʾ�˳�����
};