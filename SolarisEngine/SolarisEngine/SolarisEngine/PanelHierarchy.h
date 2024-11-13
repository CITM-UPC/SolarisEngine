#ifndef __PANEL_HIERARCHY_H__
#define __PANEL_HIERARCHY_H__

#include <vector>
#include <string>
#include "GameObject.h" // Aseg�rate de incluir tu clase GameObject
#include "Panel.h" // Incluir la clase base Panel



class PanelHierarchy : public Panel { // Hereda de Panel
public:
    PanelHierarchy();
    ~PanelHierarchy();

    void Render() override; // Implementa el m�todo Render
    void RenderContext() override; // Implementa el m�todo Render

private:
    void DrawGameObject(GameObject* gameObject); // M�todo para dibujar un GameObject
    std::string selectedItem;
};

#endif // __PANEL_HIERARCHY_H__
