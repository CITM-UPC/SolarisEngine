#pragma once
#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"

class Component_Mesh : public Component {
public:
    Component_Mesh(std::shared_ptr<GameObject> containerGO);
    virtual ~Component_Mesh() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    // Métodos específicos para el mesh
    void LoadMesh(const std::string& path);
};

#endif // !__COMPONENT_MESH_H__
