#include "Component_Mesh.h"
#include <iostream>

Component_Mesh::Component_Mesh(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Mesh) {}

Component_Mesh::~Component_Mesh() {}

void Component_Mesh::Enable() {
    enabled = true;
    // Lógica para habilitar el componente
}

void Component_Mesh::Disable() {
    enabled = false;
    // Lógica para deshabilitar el componente
}

void Component_Mesh::Update(double dt) {
    // Lógica de actualización
}

void Component_Mesh::DrawComponent() {
    // Lógica para dibujar el mesh
}
