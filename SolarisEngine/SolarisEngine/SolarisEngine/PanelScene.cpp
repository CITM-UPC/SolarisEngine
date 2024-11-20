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
         width = availableRegion.x;
         height = availableRegion.y;

        if (app->windowEditor->GetFrameBuffer()) {
            // Rescalamos el framebuffer para que coincida con el �rea del panel
            app->windowEditor->GetFrameBuffer()->RescaleFrameBuffer(width, height);
            UpdateHoveredStatus();

            // Obtener las dimensiones actuales del framebuffer
            float fbWidth = width;   // Usamos el tama�o de la regi�n disponible
            float fbHeight = height; // Usamos el tama�o de la regi�n disponible
            float fbAspect = fbWidth / fbHeight;
            float panelAspect = width / height;

            // Ajustar el tama�o de la imagen en funci�n de la relaci�n de aspecto
            ImVec2 imageSize;
            if (panelAspect > fbAspect) {
                // El panel es m�s ancho que el framebuffer, limitamos el ancho
                imageSize.x = height * fbAspect;
                imageSize.y = height;
            }
            else {
                // El panel es m�s alto que el framebuffer, limitamos la altura
                imageSize.x = width;
                imageSize.y = width / fbAspect;
            }

            // Centrar la imagen en el panel
            ImVec2 offset((width - imageSize.x) / 2.0f, (height - imageSize.y) / 2.0f);
            ImVec2 calc(ImGui::GetCursorPos().x + offset.x, ImGui::GetCursorPos().y + offset.y);
            //ImVec2 calc(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
            printf("calc.x: %f, calc.y: %f\n", calc.x, calc.y);
            ImGui::SetCursorPos(calc);

            // Renderiza la textura del framebuffer en el panel ImGui
            ImGui::Image(
                (ImTextureID)app->windowEditor->GetFrameBuffer()->getFrameTexture(),
                imageSize,    // Usa el tama�o ajustado con la relaci�n de aspecto
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }

        ImGui::EndChild();
    }
    PlayPauseOverlay();
    if (isHovered) {
        app->actualScene->isScenePicked = true;
    }
    else
    {
        app->actualScene->isScenePicked = false;
    }
    ImGui::End();  // Finaliza el panel "Scene"
}


void PanelScene::RenderContext()
{
}

void PanelScene::PlayPauseOverlay()
{
    // Crear un overlay para los botones en la esquina superior derecha
    ImGui::SetNextWindowBgAlpha(0.5f);  // Transparencia del fondo
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + width - 102, ImGui::GetWindowPos().y + 30)); // Posici�n en la esquina superior derecha
    ImGui::SetNextWindowSize(ImVec2(107, 35)); // Tama�o aproximado del panel

    ImGui::Begin("PlayPauseOverlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        if (ImGui::Button(u8" \ue0A9  ")) {
            // Acci�n de "Play"
        }
        ImGui::SameLine();
        if (ImGui::Button(u8" \ue09E  ")) {
            // Acci�n de "Pause"
        }
    }
    ImGui::End();
}

ImVec2 PanelScene::GetMousePos()
{
    return ImGui::GetMousePos();
}
