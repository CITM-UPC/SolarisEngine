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
        // Extraemos la posición de la cámara de la matriz de vista inversa
        glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
        glm::vec3 cameraPosition = glm::vec3(inverseViewMatrix[3]);

        // Obtenemos la posición del objeto
        Component_Transform* transform = containerGO->GetComponent<Component_Transform>();
        if (!transform) return glm::mat4(1.0f); // Fallback si no hay transform

        glm::vec3 objectPosition = transform->GetPosition();

        // Calcular el vector hacia la cámara (eje Z)
        glm::vec3 forward = glm::normalize(cameraPosition - objectPosition);

        // Recalcular el eje X (derecha) usando el eje Y global (up)
        const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));

        // Recalcular el eje Y usando los otros dos
        glm::vec3 up = glm::normalize(glm::cross(forward, right));

        // Construir la matriz de rotación usando los vectores calculados
        result[0] = glm::vec4(right, 0.0f);   // Primera columna: X
        result[1] = glm::vec4(up, 0.0f);      // Segunda columna: Y
        result[2] = glm::vec4(forward, 0.0f); // Tercera columna: Z
        result[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Cuarta columna: sin traslación
    }
    else if (type == BillboardType::WORLD_ALIGNED) {
        // Cálculo del World-Aligned Billboard

        // Mantener el eje Y fijo en el eje global
        const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

        // Extraer posiciones del objeto y la cámara
        glm::vec3 cameraPosition = glm::vec3(glm::inverse(viewMatrix)[3]);
        glm::vec3 objectPosition = containerGO->GetComponent<Component_Transform>()->GetPosition();

        // Calcular el eje Forward (hacia la cámara, restringido en el plano XZ)
        glm::vec3 forward = glm::normalize(glm::vec3(cameraPosition.x - objectPosition.x, 0.0f, cameraPosition.z - objectPosition.z));

        // Calcular el eje Right (perpendicular a World-Up y Forward)
        glm::vec3 right = glm::normalize(glm::cross(worldUp, forward));

        // Recalcular el eje Up (asegurando ortonormalidad)
        glm::vec3 up = glm::normalize(glm::cross(forward, right));

        // Construir la matriz final
        result[0] = glm::vec4(right, 0.0f);   // Primera columna: X
        result[1] = glm::vec4(worldUp, 0.0f); // Segunda columna: Y (constante)
        result[2] = glm::vec4(forward, 0.0f); // Tercera columna: Z
        result[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Cuarta columna (sin traslación)
    }
    else if (type == BillboardType::AXIS_ALIGNED) {
        glm::vec3 cameraPosition = glm::vec3(glm::inverse(viewMatrix)[3]); // Posición de la cámara
        glm::vec3 objectPosition = containerGO->GetComponent<Component_Transform>()->GetPosition();

        // Calcular el vector hacia la cámara
        glm::vec3 toCamera = glm::normalize(cameraPosition - objectPosition);

        // Obtener el eje desde la configuración
        glm::vec3 axis = GetAxis();

        // Restringir el vector "toCamera" al plano perpendicular al eje
        glm::vec3 forward = glm::normalize(toCamera - glm::dot(toCamera, axis) * axis);

        // Calcular el eje Right (perpendicular a Forward y al eje)
        glm::vec3 right = glm::normalize(glm::cross(axis, forward));

        // El eje Up es el mismo que el eje de restricción
        glm::vec3 up = axis;

        // Construir la matriz final
        result[0] = glm::vec4(right, 0.0f);   // Primera columna: X
        result[1] = glm::vec4(up, 0.0f);     // Segunda columna: eje de restricción
        result[2] = glm::vec4(forward, 0.0f); // Tercera columna: Z
        result[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Cuarta columna (sin traslación)
    }


    return result;
}




void Component_Billboard::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader(u8"\ue0e0 Billboard")) {
        const char* types[] = { "Screen-Aligned", "World-Aligned", "Axis-Aligned"};
        int currentType = static_cast<int>(type);
        if (ImGui::Combo("Type", &currentType, types, IM_ARRAYSIZE(types))) {
            type = static_cast<BillboardType>(currentType);
        }

        if (type == BillboardType::AXIS_ALIGNED) {
            // Opciones de alineación rápida (X, Y, Z)
            const char* axisOptions[] = { "X Axis", "Y Axis", "Z Axis", "Custom Axis" };
            int currentAxis = static_cast<int>(axisAlignment);
            if (ImGui::Combo("Axis Alignment", &currentAxis, axisOptions, IM_ARRAYSIZE(axisOptions))) {
                axisAlignment = static_cast<AxisAlignment>(currentAxis);
                UpdateAxisFromAlignment(); // Actualizar eje al seleccionar una opción
            }

            // Si se selecciona "Custom Axis", permitir que el usuario ingrese el eje
            if (axisAlignment == AxisAlignment::CUSTOM_AXIS) {
                float axisArray[3] = { customAxis.x, customAxis.y, customAxis.z };
                if (ImGui::DragFloat3("Custom Axis", axisArray, 0.01f)) {
                    SetCustomAxis(glm::vec3(axisArray[0], axisArray[1], axisArray[2]));
                }
            }
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


void Component_Billboard::SetAxisAlignment(AxisAlignment newAxisAlignment) {
    axisAlignment = newAxisAlignment;
    UpdateAxisFromAlignment();
}

void Component_Billboard::SetCustomAxis(const glm::vec3& newAxis) {
    customAxis = glm::normalize(newAxis); // Normalizar para evitar problemas
}

glm::vec3 Component_Billboard::GetAxis() const {
    if (axisAlignment == AxisAlignment::CUSTOM_AXIS) {
        return customAxis;
    }
    return GetAlignedAxis();
}

glm::vec3 Component_Billboard::GetAlignedAxis() const {
    switch (axisAlignment) {
    case AxisAlignment::X_AXIS: return glm::vec3(1.0f, 0.0f, 0.0f);
    case AxisAlignment::Y_AXIS: return glm::vec3(0.0f, 1.0f, 0.0f);
    case AxisAlignment::Z_AXIS: return glm::vec3(0.0f, 0.0f, 1.0f);
    default: return glm::vec3(0.0f, 1.0f, 0.0f); // Valor por defecto (Y)
    }
}

// Método que actualiza el eje personalizado basado en la selección del eje
void Component_Billboard::UpdateAxisFromAlignment() {
    switch (axisAlignment) {
    case AxisAlignment::X_AXIS:
        customAxis = glm::vec3(1.0f, 0.0f, 0.0f);
        break;
    case AxisAlignment::Y_AXIS:
        customAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        break;
    case AxisAlignment::Z_AXIS:
        customAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        break;
    default:
        break;
    }
}


