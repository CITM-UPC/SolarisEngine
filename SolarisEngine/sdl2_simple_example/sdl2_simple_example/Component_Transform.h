#pragma once
#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include <glm/glm.hpp>                 // Incluye GLM para vectores
#include <glm/gtc/quaternion.hpp>      // Incluye GLM para cuaterniones
#include <glm/gtc/matrix_transform.hpp> // Para transformaciones
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    void SetScale(float x, float y, float z);
    void SetRotation(float angle, float x, float y, float z);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetScale() const;
    glm::quat GetRotation() const;

    glm::mat4 GetModelMatrix() const;


private:
    glm::vec3 position;  // Posición
    glm::vec3 scale;     // Escala
    glm::quat rotationQuat; // Rotación en cuaterniones
};

#endif // !__COMPONENT_TRANSFORM_H__
