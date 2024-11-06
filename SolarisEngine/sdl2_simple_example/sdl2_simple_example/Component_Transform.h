#pragma once
#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Defs.h"

class Component_Transform : public Component {
public:
    Component_Transform(GameObject* containerGO);
    virtual ~Component_Transform() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;
    void DrawInspectorComponent() override;

    // Métodos específicos para la transformación
    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& vec3);
    void SetScale(float x, float y, float z);

    // Sobrecarga de rotación para cuaterniones y Euler
    void SetRotation(float pitch, float yaw, float roll); // Para ángulos de Euler
    void SetRotation(const glm::quat& quat); // Para cuaternión directo

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetScale() const;
    float GetRelativeSize() const;
    glm::vec3 GetRotation() const; // Devuelve los ángulos de Euler en grados

    glm::mat4 GetModelMatrix() const;

    Component* Clone() const override;

private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotationQuat;

    glm::vec3 eulerRotation;
};

#endif // !__COMPONENT_TRANSFORM_H__
