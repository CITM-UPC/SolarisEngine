#include "PanelInspector.h"
#include "imgui.h"

PanelInspector::PanelInspector() {
    // Inicialización si es necesaria
}

PanelInspector::~PanelInspector() {
    // Limpieza si es necesaria
}

void PanelInspector::Render() {
    ImGui::Begin("Inspector"); // Nombre del panel

    ImGui::Text("Inspector Panel Content Here"); // Contenido de ejemplo

    ImGui::End(); // Finaliza el panel
}
