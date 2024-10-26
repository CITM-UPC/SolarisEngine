#include "Component_Transform.h"


Component_Transform::Component_Transform(std::shared_ptr<GameObject> containerGO)
    : Component(containerGO, ComponentType::Transform),
    position(0.0f, 0.0f, 0.0f),  // Inicializa en el origen
    scale(1.0f, 1.0f, 1.0f),     // Escala por defecto
    rotationQuat(glm::quat())    // Rotación por defecto
{}

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

void Component_Transform::DrawComponent()
{
}

void Component_Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Component_Transform::SetPosition(vec3 vec3)
{
    position = vec3;
}

void Component_Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

void Component_Transform::SetRotation(float angle, float x, float y, float z) {
    glm::vec3 axis(x, y, z);
    rotationQuat = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
}

const glm::vec3& Component_Transform::GetPosition() const {
    return position;
}

const glm::vec3& Component_Transform::GetScale() const {
    return scale;
}

glm::quat Component_Transform::GetRotation() const {
    return rotationQuat;
}

glm::mat4 Component_Transform::GetModelMatrix() const {
    // Calcula y retorna la matriz de transformación completa
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);

    return translationMatrix * rotationMatrix * scaleMatrix;
}
