// GameObject.inl

#include "GameObject.h"
template <typename TComponent>
TComponent* GameObject::GetComponent() {
    for (const auto& component : components) {
        TComponent* castedComponent = dynamic_cast<TComponent*>(component.get());
        if (castedComponent) {
            return castedComponent; // Retorna el componente si el casting es exitoso
        }
    }
    return nullptr; // Si no se encuentra, retorna nullptr
}

template <typename TComponent>
TComponent* GameObject::AddComponent() {
    if (GetComponent<TComponent>() != nullptr) {
        return nullptr;  // El componente ya existe
    }

    auto newComponent = std::make_unique<TComponent>(shared_from_this());
    newComponent->Enable();  // Habilita el componente si es necesario

    // Almacena el nuevo componente en la lista
    TComponent* componentPtr = newComponent.get(); // Guardamos el puntero crudo
    components.push_back(std::move(newComponent));

    return componentPtr; // Devolvemos el puntero crudo
}
