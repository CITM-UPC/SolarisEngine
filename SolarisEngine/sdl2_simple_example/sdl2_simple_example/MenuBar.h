#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__

#include <imgui.h>
#include <imgui_impl_sdl2.h>

class MenuBar {
public:
    MenuBar() = default;
    ~MenuBar() = default;

    void Render(); // Método para renderizar la barra de menú

    bool showDemo = false;

private:
    

};

#endif // __MENU_BAR_H__
