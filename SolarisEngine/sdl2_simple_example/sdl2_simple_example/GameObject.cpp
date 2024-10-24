#include "GameObject.h"
#include "Component.h" // Asegúrate de que esta cabecera esté presente
#include "Defs.h"     // Asegúrate de que esta cabecera esté presente
#include <iostream>

// Constructor por defecto
GameObject::GameObject()
    : name("gameObject"), isStatic(false), enabled(true) {
    CreateUID(); // Genera un UID al crear el GameObject
}

// Constructor con nombre
GameObject::GameObject(std::string name)
    : name(std::move(name)), isStatic(false), enabled(true) {
    CreateUID(); // Genera un UID al crear el GameObject
}

// Destructor
GameObject::~GameObject() {
    // Aquí podrías limpiar recursos si es necesario
}

void GameObject::Awake(double dt)
{
}

void GameObject::Start(double dt)
{
}

// Actualiza el GameObject
void GameObject::Update(double dt) {
    // Lógica de actualización, si corresponde
    for (const auto& component : components) {
        if (component->IsEnabled()) {
            component->Update(dt);
        }
    }
}

void GameObject::LateUpdate(double dt)
{
}

// Dibuja el GameObject
void GameObject::Draw() {
    for (const auto& component : components) {
        if (component->IsEnabled()) {
            component->DrawComponent();
        }
    }
}

// Remueve un componente por tipo
void GameObject::RemoveComponent(ComponentType type) {
    components.erase(std::remove_if(components.begin(), components.end(),
        [type](const std::unique_ptr<Component>& component) {
            return component->GetType() == type;
        }), components.end());
}

// Comprueba si el GameObject está habilitado
bool GameObject::IsEnabled() const {
    return enabled;
}

// Habilita el GameObject
void GameObject::Enable() {
    enabled = true;
}

// Deshabilita el GameObject
void GameObject::Disable() {
    enabled = false;
}

// Marca el GameObject para ser eliminado
void GameObject::Delete() {
    // Aquí podrías agregar lógica para manejar la eliminación
}

// Obtiene el nombre del GameObject
std::string GameObject::GetName() const {
    return name;
}

// Establece el nombre del GameObject
void GameObject::SetName(const std::string& name) {
    this->name = name;
}

// Comprueba si el GameObject es estático
bool GameObject::IsStatic() const {
    return isStatic;
}

// Establece si el GameObject es estático
void GameObject::SetStatic(bool isStatic) {
    this->isStatic = isStatic;
}

// Genera un UID único
void GameObject::CreateUID() {
    UID = UIDGen::GenerateRandomUint32(); // Asegúrate de que UIDGen esté correctamente referenciado
}
