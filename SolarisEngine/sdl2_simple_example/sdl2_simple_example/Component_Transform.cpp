#include "Component_Transform.h"
#include <iostream>

Component_Transform::Component_Transform(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Transform) {}

Component_Transform::~Component_Transform() {}

void Component_Transform::Enable() {
    enabled = true;
}

void Component_Transform::Disable() {
    enabled = false;
}

void Component_Transform::Update(double dt) {
    // Lógica de actualización
}

void Component_Transform::DrawComponent() {
    // Lógica para dibujar la transformación
}

void Component_Transform::SetPosition(float x, float y, float z) {
    // Lógica para establecer la posición
}
