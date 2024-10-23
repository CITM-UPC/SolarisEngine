#include "Component.h"
#include "GameObject.h"

// Constructor
Component::Component(std::shared_ptr<GameObject> containerGO, ComponentType type)
    : containerGO(std::move(containerGO)), type(type), enabled(true) {
    CreateNameFromType(type);
    CreateUID(); // Genera un UID al crear el componente
}

// Destructor
Component::~Component() {
    // Aquí podrías limpiar recursos si es necesario
}

// Obtiene el nombre del componente
std::string Component::GetName() {
    return name;
}

// Crea un nombre a partir del tipo de componente
void Component::CreateNameFromType(ComponentType type) {
    switch (type) {
    case ComponentType::Transform: name = "Transform"; break;
    case ComponentType::Camera: name = "Camera"; break;
    case ComponentType::Mesh: name = "Mesh"; break;
    case ComponentType::Texture: name = "Texture"; break;
    case ComponentType::Material: name = "Material"; break;
    default: name = "Unknown"; break;
    }
}

// Obtiene el tipo de componente
ComponentType Component::GetType() const {
    return type;
}

// Obtiene el GameObject contenedor
std::shared_ptr<GameObject> Component::GetContainerGO() const {
    return containerGO.lock(); // Devuelve el GameObject si todavía es válido
}

// Comprueba si el componente está habilitado
bool Component::IsEnabled() const {
    return enabled;
}
