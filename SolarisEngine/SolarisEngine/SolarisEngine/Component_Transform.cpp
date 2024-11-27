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
    rotationQuat(glm::quat()) {
    eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));
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
    // Implementación de dibujo si es necesario
}

void Component_Transform::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader(u8"\ue096 Transform")) {

        glm::vec3 position = this->GetPosition();
        ImGui::DragFloat3("Position", &position[0], 0.1f);
        this->SetPosition(position);

        // Usa eulerRotation directamente y actualiza el cuaternión solo si cambia
        if (ImGui::DragFloat3("Rotation", &eulerRotation[0])) {
            glm::vec3 radiansEuler = glm::radians(eulerRotation);
            rotationQuat = glm::quat(radiansEuler);
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

void Component_Transform::SetParent(GameObject* newParent) {
    if (newParent) {
        // Guardar el offset actual relativo al nuevo padre
        Component_Transform* parentTransform = newParent->GetComponent<Component_Transform>();
        if (parentTransform) {
            // Calcular el offset de la posición, rotación y escala
            localPositionOffset = position - parentTransform->GetPosition();
            localRotationOffset = glm::inverse(parentTransform->rotationQuat);
            localScaleOffset = scale - parentTransform->GetScale();
        }
    }
    else {
        // Si el padre es nulo, simplemente reseteamos el offset
        localPositionOffset = glm::vec3(0.0f);
        localRotationOffset = glm::quat();
        localScaleOffset = glm::vec3(1.0f);
    }
}

glm::mat4 Component_Transform::GetModelMatrix() const {

    if (containerGO->parent) {
        glm::mat4 parentModelMatrix = glm::mat4(1.0f); // Si no tiene padre, usa la matriz identidad
        if (containerGO->parent != nullptr) {
            Component_Transform* parentTransform = containerGO->parent->GetComponent<Component_Transform>();
            if (parentTransform) {
                parentModelMatrix = parentTransform->GetModelMatrix();
            }
        }

        // Crear las matrices de transformación locales (traslación, rotación, escala) usando el offset
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), localPositionOffset + position);
        glm::mat4 rotationMatrix = glm::mat4_cast(localRotationOffset + rotationQuat);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), localScaleOffset + scale);

        // Multiplicar la matriz del padre con la transformación local (con el offset)
        return parentModelMatrix * translationMatrix * rotationMatrix * scaleMatrix;
    }
    else {
        // Si no tiene padre, la matriz global es solo la combinación de su propia transformación local
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);  // usa `position`, no el offset
        glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);  // usa `rotationQuat`, no el offset
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);  // usa `scale`, no el offset

        // Matriz global para un objeto sin padre es simplemente la multiplicación de las transformaciones locales
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    
}

void Component_Transform::ApplyMatrix(const glm::mat4& matrix) {
    // Extraer posición
    position = glm::vec3(matrix[3]);

    // Extraer escala
    scale = glm::vec3(
        glm::length(glm::vec3(matrix[0])),
        glm::length(glm::vec3(matrix[1])),
        glm::length(glm::vec3(matrix[2]))
    );

    // Extraer rotación
    glm::mat3 rotationMatrix = glm::mat3(
        glm::vec3(matrix[0]) / scale.x,
        glm::vec3(matrix[1]) / scale.y,
        glm::vec3(matrix[2]) / scale.z
    );
    rotationQuat = glm::quat_cast(rotationMatrix);

    // Actualizar eulerRotation para reflejar la nueva rotación
    eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));
}

Component* Component_Transform::Clone() const {
    return new Component_Transform(*this);
}
