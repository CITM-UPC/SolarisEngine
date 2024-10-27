#include "MenuBar.h"
#include <SDL2/SDL.h>

void MenuBar::Render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("Exit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Primitive")) {
            if (ImGui::MenuItem("Cube")) {
                // Aquí puedes implementar la aparición del cubo.
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
