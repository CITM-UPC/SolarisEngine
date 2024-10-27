#pragma once
#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include <glm/glm.hpp>                 // Incluye GLM para vectores
#include <glm/gtc/quaternion.hpp>      // Incluye GLM para cuaterniones
#include <glm/gtc/matrix_transform.hpp> // Para transformaciones
#include <glm/gtc/type_ptr.hpp>

#include "Defs.h"

class Component_Transform : public Component {
public:
    Component_Transform(GameObject* containerGO); // Cambiado a puntero crudo
    virtual ~Component_Transform() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    // Métodos específicos para la transformación
    void SetPosition(float x, float y, float z);
    void SetPosition(glm::vec3 vec3); // Asegúrate de usar glm::vec3
    void SetScale(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetScale() const;
    glm::vec3 GetRotation() const;

    glm::mat4 GetModelMatrix() const;

private:
    glm::vec3 position;  // Posición
    glm::vec3 scale;     // Escala
    glm::quat rotationQuat; // Rotación en cuaterniones
};

#endif // !__COMPONENT_TRANSFORM_H__
