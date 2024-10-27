#include "PanelInspector.h"
#include "imgui.h"

PanelInspector::PanelInspector() {
    // Inicializaci�n si es necesaria
}

PanelInspector::~PanelInspector() {
    // Limpieza si es necesaria
}

void PanelInspector::Render() {
    ImGui::Begin("Inspector"); // Nombre del panel

    if (selectedGameObject) {
        ImGui::Text("Inspector for: %s", selectedGameObject->GetName().c_str()); // Muestra el nombre del GameObject seleccionado

        // Renderiza los componentes del GameObject
        for (auto component : selectedGameObject->GetComponents()) { // Asume que tienes un m�todo GetComponents()
            DrawComponent(component);
        }
    }
    else {
        ImGui::Text("No GameObject selected."); // Mensaje si no hay ning�n GameObject seleccionado
    }

    ImGui::End(); // Finaliza el panel
}

void PanelInspector::SetSelectedGameObject(GameObject* gameObject)
{
    selectedGameObject = gameObject; // Guarda el GameObject seleccionado
}

void PanelInspector::DrawComponent(Component* component) {
    // Muestra informaci�n del componente
    if (ImGui::CollapsingHeader(component->GetName().c_str())) { // Crea una pesta�a desplegable
        // Aqu� puedes a�adir l�gica para editar componentes, por ejemplo:
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
            ImGui::DragFloat3("Position", &position[0]); // Editor de posici�n
            transform->SetPosition(position); // Actualiza la posici�n

            glm::vec3 scale = transform->GetScale();
            ImGui::DragFloat3("Scale", &scale[0]); // Editor de escala
            transform->SetScale(scale.x, scale.y, scale.z); // Actualiza la escala

            glm::vec3 rotation = transform->GetRotation();
            ImGui::DragFloat3("Rotation", &rotation[0]); // Editor de rotaci�n
            transform->SetRotation(rotation.x, rotation.y, rotation.z); // Actualiza la rotaci�n
            break;
        }
                                     // Puedes agregar m�s tipos de componentes aqu�, siguiendo el mismo patr�n
        default:
            ImGui::Text("Unknown Component Type");
            break;
        }
    }
}
