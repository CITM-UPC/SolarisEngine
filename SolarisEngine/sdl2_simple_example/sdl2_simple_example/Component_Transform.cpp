#include "Component_Transform.h"

Component_Transform::Component_Transform(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Transform),
    position(0.0f, 0.0f, 0.0f),  // Inicializa en el origen
    scale(1.0f, 1.0f, 1.0f),     // Escala por defecto
    rotationQuat(glm::quat())    // Rotación por defecto
{
}

Component_Transform::~Component_Transform() {}

void Component_Transform::Enable() {
    enabled = true;
}

void Component_Transform::Disable() {
    enabled = false;
}

void Component_Transform::Update(double dt) {
    // Lógica de actualización, si es necesario
}

void Component_Transform::DrawComponent() {
    // Aquí podrías aplicar las transformaciones al modelo
}

void Component_Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Component_Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

void Component_Transform::SetRotation(float angle, float x, float y, float z) {
    glm::vec3 axis(x, y, z);
    rotationQuat = glm::angleAxis(glm::radians(angle), glm::normalize(axis)); // Crear el cuaternión
}

const glm::vec3& Component_Transform::GetPosition() const {
    return position;
}

const glm::vec3& Component_Transform::GetScale() const {
    return scale;
}

glm::quat Component_Transform::GetRotation() const {
    return rotationQuat; // Retorna el cuaternión
}
