#include "Component_Transform.h"
#include "App.h"

Component_Transform::Component_Transform(GameObject* containerGO) // Cambiado a puntero crudo
    : Component(containerGO, ComponentType::Transform),
    position(0.0f, 0.0f, 0.0f),  // Inicializa en el origen
    scale(1.0f, 1.0f, 1.0f),     // Escala por defecto
    rotationQuat(glm::quat())    // Rotaci�n por defecto
{}

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

void Component_Transform::DrawComponent()
{
    // Implementaci�n de dibujo si es necesario
}

void Component_Transform::DrawInspectorComponent()
{
    ImGui::Text("Transform Properties");
    glm::vec3 position = this->GetPosition();
    ImGui::DragFloat3("Position", &position[0]); // Editor de posici�n
    this->SetPosition(position); // Actualiza la posici�n

    glm::vec3 scale = this->GetScale();
    ImGui::DragFloat3("Scale", &scale[0]); // Editor de escala
    this->SetScale(scale.x, scale.y, scale.z); // Actualiza la escala

    glm::vec3 rotation = this->GetRotation();
    ImGui::DragFloat3("Rotation", &rotation[0]); // Editor de rotaci�n
    this->SetRotation(rotation.x, rotation.y, rotation.z); // Actualiza la rotaci�n
}

void Component_Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Component_Transform::SetPosition(glm::vec3 vec3) // Aseg�rate de usar glm::vec3
{
    position = vec3;
}

void Component_Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

// Modificado para aceptar �ngulos de Euler (pitch, yaw, roll)
void Component_Transform::SetRotation(float pitch, float yaw, float roll) {
    // Convertir �ngulos de Euler a cuaterni�n
    glm::vec3 eulerAngles(glm::radians(pitch), glm::radians(yaw), glm::radians(roll));
    rotationQuat = glm::quat(eulerAngles); // Convertir a cuaterni�n
}

const glm::vec3& Component_Transform::GetPosition() const {
    return position;
}

const glm::vec3& Component_Transform::GetScale() const {
    return scale;
}

float Component_Transform::GetRelativeSize() const {
    // Obtener el componente de malla asociado
    Component_Mesh* mesh = containerGO->GetComponent<Component_Mesh>();
    if (!mesh) {
        return 1.0f; // Valor predeterminado si no hay malla
    }

    // Calcular el tama�o de la malla
    glm::vec3 meshSize = mesh->CalculateMeshSize();

    // Aplicar la escala del transformador al tama�o de la malla
    glm::vec3 scaledSize = meshSize * scale;

    // Retornar el mayor valor en los tres ejes como tama�o relativo
    return glm::max(scaledSize.x, glm::max(scaledSize.y, scaledSize.z));
}



glm::vec3 Component_Transform::GetRotation() const {
    glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);
    return glm::degrees(eulerAngles); // Convertir a grados
}

glm::mat4 Component_Transform::GetModelMatrix() const {
    // Calcula y retorna la matriz de transformaci�n completa
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotationMatrix = glm::mat4_cast(rotationQuat);

    return translationMatrix * rotationMatrix * scaleMatrix;
}
