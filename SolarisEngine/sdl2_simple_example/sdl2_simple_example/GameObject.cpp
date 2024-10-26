#include "GameObject.h"
#include "Component.h"
#include "Component_Transform.h"
#include <iostream>

// Método estático para crear el GameObject e inicializarlo con un Component_Transform
std::shared_ptr<GameObject> GameObject::Create(const std::string& name) {
    auto gameObject = std::shared_ptr<GameObject>(new GameObject(name));
    gameObject->AddComponent<Component_Transform>();  // Agrega el componente Transform
    return gameObject;
}

// Constructor privado
GameObject::GameObject(const std::string& name)
    : name(name), isStatic(false), enabled(true) {
    CreateUID();
}

// Destructor
GameObject::~GameObject() {
    // Limpieza de recursos, si es necesario
    components.clear();
}

void GameObject::Awake(double dt) { }

void GameObject::Start(double dt) { }

void GameObject::Update(double dt) {
    for (const auto& component : components) {
        if (component && component->IsEnabled()) {
            component->Update(dt);
        }
    }
}

void GameObject::LateUpdate(double dt) { }

void GameObject::Draw() {
    for (const auto& component : components) {
        if (component && component->IsEnabled()) {
            component->DrawComponent();
        }
    }
}

void GameObject::RemoveComponent(ComponentType type) {
    components.erase(std::remove_if(components.begin(), components.end(),
        [type](const std::unique_ptr<Component>& component) {
            return component->GetType() == type;
        }), components.end());
}

bool GameObject::IsEnabled() const { return enabled; }

void GameObject::Enable() { enabled = true; }

void GameObject::Disable() { enabled = false; }

void GameObject::Delete() { 
    Disable();
    components.clear();
}

std::string GameObject::GetName() const { return name; }

void GameObject::SetName(const std::string& name) { this->name = name; }

bool GameObject::IsStatic() const { return isStatic; }

void GameObject::SetStatic(bool isStatic) { this->isStatic = isStatic; }

void GameObject::CreateUID() {
    UID = UIDGen::GenerateRandomUint32();
}
