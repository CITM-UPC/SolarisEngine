#pragma once
#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"

class Component_Transform : public Component {
public:
    Component_Transform(std::shared_ptr<GameObject> containerGO);
    virtual ~Component_Transform() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    // Métodos específicos para la transformación
    void SetPosition(float x, float y, float z);
};

#endif // !__COMPONENT_TRANSFORM_H__
