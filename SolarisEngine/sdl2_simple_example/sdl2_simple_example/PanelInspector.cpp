#include "PanelInspector.h"
#include "imgui.h"

PanelInspector::PanelInspector() {
    // Inicialización si es necesaria
}

PanelInspector::~PanelInspector() {
    // Limpieza si es necesaria
}

void PanelInspector::Render() {
    ImGui::Begin("Inspector"); // Nombre del panel

    if (selectedGameObject) {
        ImGui::Text("Inspector for: %s", selectedGameObject->GetName().c_str()); // Muestra el nombre del GameObject seleccionado

        // Renderiza los componentes del GameObject
        for (auto component : selectedGameObject->GetComponents()) { // Asume que tienes un método GetComponents()
            DrawComponent(component);
        }
    }
    else {
        ImGui::Text("No GameObject selected."); // Mensaje si no hay ningún GameObject seleccionado
    }

    ImGui::End(); // Finaliza el panel
}

void PanelInspector::SetSelectedGameObject(GameObject* gameObject)
{
    selectedGameObject = gameObject; // Guarda el GameObject seleccionado
}

void PanelInspector::DrawComponent(Component* component) {
    // Muestra información del componente
    if (ImGui::CollapsingHeader(component->GetName().c_str())) { // Crea una pestaña desplegable
        // Aquí puedes añadir lógica para editar componentes, por ejemplo:
        switch (component->GetType()) {
        case ComponentType::Material: {
            Component_Material* material = static_cast<Component_Material*>(component);
            float color[3] = { material->GetDiffuseColor()[0], material->GetDiffuseColor()[1], material->GetDiffuseColor()[2] };

            ImGui::Text("Material Properties");
            ImGui::ColorEdit3("Diffuse Color", color); // Editor de color para el color difuso
            material->SetDiffuseColor(color[0], color[1], color[2]); // Actualiza el color difuso
            break;
        }
        case ComponentType::Transform: {
            Component_Transform* transform = static_cast<Component_Transform*>(component);

            ImGui::Text("Transform Properties");
            glm::vec3 position = transform->GetPosition();
            ImGui::DragFloat3("Position", &position[0]); // Editor de posición
            transform->SetPosition(position); // Actualiza la posición

            glm::vec3 scale = transform->GetScale();
            ImGui::DragFloat3("Scale", &scale[0]); // Editor de escala
            transform->SetScale(scale.x, scale.y, scale.z); // Actualiza la escala

            glm::vec3 rotation = transform->GetRotation();
            ImGui::DragFloat3("Rotation", &rotation[0]); // Editor de rotación
            transform->SetRotation(rotation.x, rotation.y, rotation.z); // Actualiza la rotación
            break;
        }
                                     // Puedes agregar más tipos de componentes aquí, siguiendo el mismo patrón
        default:
            ImGui::Text("Unknown Component Type");
            break;
        }
    }
}
