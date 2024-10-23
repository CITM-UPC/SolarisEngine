#include "Component_Material.h"
#include <iostream>

Component_Material::Component_Material(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Material) {}

Component_Material::~Component_Material() {}

void Component_Material::Enable() {
    enabled = true;
}

void Component_Material::Disable() {
    enabled = false;
}

void Component_Material::Update(double dt) {
    // Lógica de actualización
}

void Component_Material::DrawComponent() {
    // Lógica para dibujar el material
}

void Component_Material::SetTexture(const std::string& texturePath) {
    // Lógica para establecer la textura
}
