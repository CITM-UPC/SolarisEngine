#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__

#include <vector>
#include <string>
#include "GameObject.h" // Asegúrate de incluir tu clase GameObject
#include "Panel.h" // Incluir la clase base Panel



class PanelHierarchy : public Panel { // Hereda de Panel
public:
    PanelHierarchy();
    ~PanelHierarchy();

    void Render() override; // Implementa el método Render
    void RenderContext() override; // Implementa el método Render

private:
    void DrawGameObject(GameObject* gameObject); // Método para dibujar un GameObject
    std::string selectedItem;
};

#endif // __PANEL_HIERARCHY_H__
