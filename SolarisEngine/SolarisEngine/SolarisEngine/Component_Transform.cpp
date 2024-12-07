#include "Component_Transform.h"
#include "App.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Debug.h"

Component_Transform::Component_Transform(GameObject* containerGO)
    : Component(containerGO, ComponentType::Transform),
    position(0.0f, 0.0f, 0.0f),
    scale(1.0f, 1.0f, 1.0f),
    rotationQuat(glm::quat()) {
    eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));
    localRotationOffset = glm::quat();
}

Component_Transform::~Component_Transform() {}

void Component_Transform::Enable() {
    enabled = true;
}

void Component_Transform::Disable() {
    enabled = false;
}

void Component_Transform::Update(double dt) {
    // L�gica de actualizaci�n, si es necesario
}

void Component_Transform::DrawComponent() {
    // Implementaci�n de dibujo si es necesario
}

void Component_Transform::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader(u8"\ue096 Transform")) {

        glm::vec3 position = this->GetPosition();
        ImGui::DragFloat3("Position", &position[0], 0.1f);
        this->SetPosition(position);

        // Usa eulerRotation directamente y actualiza el cuaterni�n solo si cambia
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
    glm::vec3 internalEulerAngles(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
   
    rotationQuat = glm::quat(internalEulerAngles);
    eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));

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
            // Calcular el offset de la posici�n, rotaci�n y escala
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
    // Verificar si la posici�n, rotaci�n o escala contienen valores NaN
    if (std::isnan(position.x) || std::isnan(position.y) || std::isnan(position.z)) {
        //Debug::Log("NaN detectado en posici�n.");
    }
    if (std::isnan(rotationQuat.x) || std::isnan(rotationQuat.y) || std::isnan(rotationQuat.z) || std::isnan(rotationQuat.w)) {
        //Debug::Log("NaN detectado en rotaci�n.");
    }
    if (std::isnan(scale.x) || std::isnan(scale.y) || std::isnan(scale.z)) {
        //Debug::Log("NaN detectado en escala.");
    }

    // Si el objeto tiene un padre, obtener la matriz del padre
    glm::mat4 parentModelMatrix = glm::mat4(1.0f); // Si no tiene padre, usamos la identidad
    if (containerGO->parent != nullptr) {
        Component_Transform* parentTransform = containerGO->parent->GetComponent<Component_Transform>();
        if (parentTransform) {
            parentModelMatrix = parentTransform->GetModelMatrix();
        }
    }

    glm::quat a = localRotationOffset;

    if (std::isnan(a.x) || std::isnan(a.y) || std::isnan(a.z) || std::isnan(a.w)) {
        //Debug::Log("NaN detectado en rotaci�n. Usando cuaterni�n de identidad.");
        // Si el cuaterni�n de rotaci�n es NaN, reemplazarlo por el cuaterni�n de identidad
        a = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Cuaterni�n de identidad
    }


    // Crear las matrices de transformaci�n locales (traslaci�n, rotaci�n, escala) usando el offset
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), localPositionOffset + position);
    glm::mat4 rotationMatrix = glm::mat4_cast(a + rotationQuat);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), localScaleOffset + scale);




    // Multiplicar la matriz del padre con la transformaci�n local (con el offset)
    if (containerGO->parent != nullptr) {
        return parentModelMatrix * translationMatrix * rotationMatrix * scaleMatrix;
    }
    else {
        // Si no tiene padre, la matriz global es solo la combinaci�n de su propia transformaci�n local
        return translationMatrix * rotationMatrix * scaleMatrix;
    }
}


void Component_Transform::ApplyMatrix(const glm::mat4& matrix) {
    // Extraer posici�n
    position = glm::vec3(matrix[3]);

    // Extraer escala
    scale = glm::vec3(
        glm::length(glm::vec3(matrix[0])),
        glm::length(glm::vec3(matrix[1])),
        glm::length(glm::vec3(matrix[2]))
    );

    // Extraer rotaci�n
    glm::mat3 rotationMatrix = glm::mat3(
        glm::vec3(matrix[0]) / scale.x,
        glm::vec3(matrix[1]) / scale.y,
        glm::vec3(matrix[2]) / scale.z
    );
    rotationQuat = glm::quat_cast(rotationMatrix);

    // Actualizar eulerRotation para reflejar la nueva rotaci�n
    eulerRotation = glm::degrees(glm::eulerAngles(rotationQuat));
}

Component* Component_Transform::Clone() const {
    return new Component_Transform(*this);
}
