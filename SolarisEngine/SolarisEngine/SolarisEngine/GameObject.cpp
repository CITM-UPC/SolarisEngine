#include "GameObject.h"
#include "Component.h"
#include "Component_Transform.h"
#include "App.h"
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
        child->Delete();
       
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

void GameObject::SetEnable(bool enable) { (enable) ? Enable() : Disable(); }

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

GameObject* GameObject::Duplicate() const {
    GameObject* newGameObject = new GameObject(name); // Crea un nuevo GameObject con el mismo nombre

    // Clona todos los componentes
    for (auto component : components) {
        if (component) {
            Component* newComponent = component->Clone(); // Suponiendo que cada componente tiene un método Clone()
            newGameObject->AddComponent(newComponent); // Añade el componente clonado al nuevo GameObject
        }
    }

    // Opcional: Clonar hijos (si es necesario)
    for (auto child : children) {
        if (child) {
            GameObject* newChild = child->Duplicate(); // Duplicar el hijo
            newGameObject->AddChild(newChild); // Añadir el hijo clonado al nuevo GameObject
        }
    }

    return newGameObject; // Retorna el nuevo GameObject
}

// Método para añadir un hijo (implementación simple)
void GameObject::AddChild(GameObject* child) {
    if (child) {
        children.push_back(child);
        if (child->parent != this) {
            child->SetParent(this);
        }
    }
}

void GameObject::RemoveChild(GameObject* child) {
    // Elimina un hijo de la lista de hijos
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
    }
}

std::vector<GameObject*> GameObject::GetChildren()
{
    return children;
}

// Método para establecer el padre (implementación simple)
void GameObject::SetParent(GameObject* parent) {
    // Detectar si el nuevo padre es un hijo del GameObject actual (ciclo)
    if (parent) {
        if (parent != this->parent) {

            if (this->parent) {
                this->parent->RemoveChild(this);
            }
            else {
                app->actualScene->RemoveRootGameObject(this);
            }
            

            this->parent = parent;
            this->parent->AddChild(this);
        }
    }
}
