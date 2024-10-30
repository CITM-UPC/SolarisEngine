#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h" // Incluir la clase base Panel
#include "Component.h"
#include "Component_Material.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"

class PanelInspector : public Panel { // Hereda de Panel
public:
    PanelInspector();
    ~PanelInspector();

    void Render() override; // Implementa el método Render
    void SetSelectedGameObject(GameObject* gameObject);

    GameObject* selectedGameObject;
private:
    

};

#endif // __PANEL_INSPECTOR_H__
