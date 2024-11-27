#include "Component_Billboard.h"
#include "App.h"
#include "CameraEditor.h"

Component_Billboard::Component_Billboard(GameObject* containerGO, BillboardType type)
    : Component(containerGO, ComponentType::Billboard), type(type) {}

Component_Billboard::~Component_Billboard() {}

void Component_Billboard::Update(double dt) {
    Component_Transform* transform = containerGO->GetComponent<Component_Transform>();
    if (!transform) return;

    // Obtener la matriz de vista de la cámara activa
    glm::mat4 viewMatrix = app->cameraEditor->getViewMatrix();

    // Calcular la matriz de billboard
    glm::mat4 billboardMatrix = CalculateBillboardMatrix(viewMatrix);

    // Crear la matriz final combinando traslación, rotación y escala
    glm::vec3 position = transform->GetPosition();
    glm::vec3 scale = transform->GetScale();
    glm::mat4 finalMatrix = glm::translate(glm::mat4(1.0f), position) * billboardMatrix * glm::scale(glm::mat4(1.0f), scale);

    // Aplicar la matriz al transform
    transform->ApplyMatrix(finalMatrix);
}

glm::mat4 Component_Billboard::CalculateBillboardMatrix(const glm::mat4& viewMatrix) {
    glm::mat4 result(1.0f);

    if (type == BillboardType::SCREEN_ALIGNED) {
        Component_Transform* transform = containerGO->GetComponent<Component_Transform>();
        const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);


        glm::inverse(viewMatrix);

        // Copiar las columnas rotacionales de la matriz de vista (sin traslación)
        result[0] = glm::vec4(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], 0.0f);
        result[1] = glm::vec4(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0.0f);
        result[2] = glm::vec4(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], 0.0f);


    }
    else if (type == BillboardType::WORLD_ALIGNED) {
        // Usar constantes para el eje Y en lugar de recalcular
        const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

        glm::vec3 forward = glm::normalize(glm::vec3(viewMatrix[2][0], 0.0f, viewMatrix[2][2]));
        glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));

        result[0] = glm::vec4(right, 0.0f);
        result[1] = glm::vec4(worldUp, 0.0f);
        result[2] = glm::vec4(forward, 0.0f);
    }

    return result;
}

void Component_Billboard::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader("Billboard Settings")) {
        const char* types[] = { "Screen-Aligned", "World-Aligned" };
        int currentType = static_cast<int>(type);
        if (ImGui::Combo("Type", &currentType, types, IM_ARRAYSIZE(types))) {
            type = static_cast<BillboardType>(currentType);
        }
    }
}

void Component_Billboard::SetType(BillboardType newType) {
    type = newType;
}

BillboardType Component_Billboard::GetType() const {
    return type;
}

Component* Component_Billboard::Clone() const {
    return new Component_Billboard(*this);
}

void Component_Billboard::DrawComponent() {
    // Aquí podría ir lógica para debug visual o efectos.
    Update(0);
}
void Component_Billboard::Enable() {
    enabled = true;
}

void Component_Billboard::Disable() {
    enabled = false;
}

