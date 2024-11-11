#include "PanelScene.h"
#include "imgui.h"
#include "App.h"

PanelScene::PanelScene()
{
}

PanelScene::~PanelScene()
{
}

void PanelScene::Render() {
    ImGui::Begin("Scene");  // Crea el panel "Scene"
    {
        ImGui::BeginChild("GameRender");  // Panel hijo para renderizado (opcional)

        ImVec2 availableRegion = ImGui::GetContentRegionAvail();
        float width = availableRegion.x;
        float height = availableRegion.y;

        // Asegúrate de que el framebuffer esté disponible
        if (app->windowEditor->GetFrameBuffer()) {
            // Renderiza la textura del framebuffer en el panel ImGui
            ImGui::Image(
                (ImTextureID)app->windowEditor->GetFrameBuffer()->getFrameTexture(),
                ImVec2(width, height),  // Escala la textura para que se ajuste al área disponible
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }

        ImGui::EndChild();
    }
    ImGui::End();  // Finaliza el panel "Scene"
}


void PanelScene::RenderContext()
{
}
