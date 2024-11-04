#include "Component_Transform.h"
#include "App.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

Component_Transform::Component_Transform(GameObject* containerGO)
    : Component(containerGO, ComponentType::Transform),
    position(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    rotationQuat(glm::quat()) {}

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
    // Implementación de dibujo si es necesario
}

void Component_Transform::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader(u8"\ue096 Transform")) {

        glm::vec3 position = this->GetPosition();
        ImGui::DragFloat3("Position", &position[0], 0.1f);
        this->SetPosition(position);

        // Manipulación de rotación en cuaterniones directamente para evitar gimbal lock
        glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));

        if (ImGui::DragFloat3("Rotation", &eulerRotation[0])) {
            glm::vec3 deltaRotation = glm::radians(eulerRotation) - glm::eulerAngles(rotationQuat);
            rotationQuat = glm::normalize(rotationQuat * glm::quat(deltaRotation));
        }

        glm::vec3 scale = this->GetScale();
        ImGui::DragFloat3("Scale", &scale[0]);
        this->SetScale(scale.x, scale.y, scale.z);
    }
}

void Component_Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Component_Transform::SetPosition(const glm::vec3& vec3) {
    position = vec3;
}

void Component_Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

void Component_Transform::SetRotation(float pitch, float yaw, float roll) {
    glm::vec3 eulerAngles(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
    rotationQuat = glm::quat(eulerAngles);
}

void Component_Transform::SetRotation(const glm::quat& quat) {
    rotationQuat = quat;
}

const glm::vec3& Component_Transform::GetPosition() const {
    return position;
}

const glm::vec3& Component_Transform::GetScale() const {
    return scale;
}

float Component_Transform::GetRelativeSize() const {
    Component_Mesh* mesh = containerGO->GetComponent<Component_Mesh>();
    if (!mesh) {
        return 1.0f;
    }

    glm::vec3 meshSize = mesh->CalculateMeshSize();
    glm::vec3 scaledSize = meshSize * scale;
    return glm::max(scaledSize.x, glm::max(scaledSize.y, scaledSize.z));
}

glm::vec3 Component_Transform::GetRotation() const {
    return glm::degrees(glm::eulerAngles(rotationQuat));
}

glm::mat4 Component_Transform::GetModelMatrix() const {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

Component* Component_Transform::Clone() const {
    return new Component_Transform(*this);
}
