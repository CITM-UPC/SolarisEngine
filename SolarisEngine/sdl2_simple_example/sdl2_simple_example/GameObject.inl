#include "GameObject.h"

// Método para obtener un componente de tipo TComponent
template <typename TComponent>
TComponent* GameObject::GetComponent() {
    for (const auto& component : components) {
        TComponent* castedComponent = dynamic_cast<TComponent*>(component);
        if (castedComponent) {
            return castedComponent; // Retorna el componente si el casting es exitoso
        }
    }
    return nullptr; // Si no se encuentra, retorna nullptr
}

// Método para agregar un componente de tipo TComponent
template <typename TComponent>
TComponent* GameObject::AddComponent() {
    if (GetComponent<TComponent>() != nullptr) {
        return nullptr;  // El componente ya existe
    }

    // Crea una nueva instancia del componente utilizando el constructor por defecto
    TComponent* newComponent = new TComponent(this); // Puntero crudo
    newComponent->Enable();  // Habilita el componente si es necesario

    // Almacena el nuevo componente en la lista
    components.push_back(newComponent); // Almacena el puntero crudo

    return newComponent; // Devolvemos el puntero crudo
}
