#pragma once
#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"

class Component_Material : public Component {
public:
    Component_Material(std::shared_ptr<GameObject> containerGO);
    virtual ~Component_Material() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    // Métodos específicos para el material
    void SetTexture(const std::string& texturePath);
};

#endif // !__COMPONENT_MATERIAL_H__
