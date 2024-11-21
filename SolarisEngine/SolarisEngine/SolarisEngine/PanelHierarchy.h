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
    void RenderGameObjectsRecursively(std::vector<GameObject*> gameObjects, int indentLevel = 0);
    void RenderContext() override; // Implementa el m�todo Render

    void UpdateHoveredStatus();

    void RenderSavePrefab();

private:
    void DrawGameObject(GameObject* gameObject); // M�todo para dibujar un GameObject
    std::string selectedItem;
    bool showSavePrefabPopup = false;
};

#endif // __PANEL_HIERARCHY_H__
