#pragma once
#ifndef __COMPONENT_BOUNDINGBOX_H__
#define __COMPONENT_BOUNDINGBOX_H__

#include "Component.h"
#include <glm/glm.hpp>

class Component_Transform;  // Forward declaration

class Component_BoundingBox : public Component {
public:
    Component_BoundingBox(GameObject* containerGO);
    virtual ~Component_BoundingBox() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    // Actualiza la bounding box en espacio mundial
    void UpdateBoundingBox(const glm::vec3& minLocal, const glm::vec3& maxLocal, const Component_Transform& transform);

    // Devuelve los puntos mínimos y máximos de la bounding box en espacio mundial
    const glm::vec3& GetMinPointWorld() const;
    const glm::vec3& GetMaxPointWorld() const;

    // Método para verificar si está dentro del frustum de la cámara (implementación básica)
    /*bool IsInsideFrustum(const Frustum& frustum) const;*/

private:
    glm::vec3 minLocal;  // Punto mínimo de la bounding box en espacio local
    glm::vec3 maxLocal;  // Punto máximo de la bounding box en espacio local
    glm::vec3 minWorld;  // Punto mínimo de la bounding box en espacio mundial
    glm::vec3 maxWorld;  // Punto máximo de la bounding box en espacio mundial
};

#endif // __COMPONENT_BOUNDINGBOX_H__
