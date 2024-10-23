#include "Component_Mesh.h"
#include <iostream>

Component_Mesh::Component_Mesh(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Mesh) {}

Component_Mesh::~Component_Mesh() {}

void Component_Mesh::Enable() {
    enabled = true;
    // L�gica para habilitar el componente
}

void Component_Mesh::Disable() {
    enabled = false;
    // L�gica para deshabilitar el componente
}

void Component_Mesh::Update(double dt) {
    // L�gica de actualizaci�n
}

void Component_Mesh::DrawComponent() {
    // L�gica para dibujar el mesh
}
