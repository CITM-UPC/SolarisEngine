#include "GameObject.h"
#include "Component.h"
#include "Component_Transform.h"
#include <iostream>

// Método estático para crear el GameObject e inicializarlo con un Component_Transform
GameObject* GameObject::Create(const std::string& name) {
    GameObject* gameObject(new GameObject(name));
    gameObject->AddComponent<Component_Transform>();  // Agrega el componente Transform
    return gameObject;
}

// Constructor privado
GameObject::GameObject(const std::string& name)
    : name(name), isStatic(false), enabled(true), parent(nullptr) {
    CreateUID();
}

// Destructor
GameObject::~GameObject() {
    // Limpieza de recursos
    for (auto component : components) {
        delete component; // Limpiar cada componente
    }
    components.clear();
    // Limpiar los hijos
    for (auto child : children) {
        delete child; // Limpiar cada hijo
    }
    children.clear();
}

void GameObject::Awake(double dt) { }

void GameObject::Start(double dt) { }

void GameObject::Update(double dt) {
    for (auto component : components) {
        if (component && component->IsEnabled()) {
            component->Update(dt);
        }
    }
}

void GameObject::LateUpdate(double dt) { }

void GameObject::Draw() {
    for (auto component : components) {
        if (component && component->IsEnabled()) {
            component->DrawComponent();
        }
    }
}

void GameObject::RemoveComponent(ComponentType type) {
    components.erase(std::remove_if(components.begin(), components.end(),
        [type](Component* component) {
            return component->GetType() == type;
        }), components.end());
}

std::vector<Component*> GameObject::GetComponents() {
    return components; // Devuelve la lista de componentes
}

bool GameObject::IsEnabled() const { return enabled; }

void GameObject::Enable() { enabled = true; }

void GameObject::Disable() { enabled = false; }

void GameObject::Delete() {
    Disable();
    for (auto component : components) {
        delete component; // Limpiar los componentes
    }
    components.clear();
    delete this;
}


// Otros métodos
std::string GameObject::GetName() const { return name; }

void GameObject::SetName(const std::string& name) { this->name = name; }

bool GameObject::IsStatic() const { return isStatic; }

void GameObject::SetStatic(bool isStatic) { this->isStatic = isStatic; }

void GameObject::CreateUID() {
    UID = UIDGen::GenerateRandomUint32();
}
