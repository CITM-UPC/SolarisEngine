#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Component.h"
#include "Defs.h"
#include <string>
#include <vector>
#include <memory> // Para std::unique_ptr

class GameObject {
public:
    // Método estático para crear un GameObject y agregar el Component_Transform
    static GameObject* Create(const std::string& name = "gameObject");

    ~GameObject();

    void Awake(double dt);
    void Start(double dt);
    void Update(double dt);
    void LateUpdate(double dt);
    void Draw();

    // Components
    template <typename TComponent>
    TComponent* GetComponent();

    template <typename TComponent>
    TComponent* AddComponent();

    void RemoveComponent(ComponentType type);

    std::vector<Component*> GetComponents();

    // Get/Set
    bool IsEnabled() const;
    void Enable();
    void Disable();
    void Delete(); // Método para eliminar


    std::string GetName() const;
    void SetName(const std::string& name);

    bool IsStatic() const;
    void SetStatic(bool isStatic);

    void CreateUID();
    uint32_t GetUID() { return UID; }

private:
    // Constructor privado para que solo pueda usarse a través de Create()
    GameObject(const std::string& name = "gameObject");

public:
    GameObject* parent; // Cambiado a puntero crudo
    std::vector<GameObject*> children; // Cambiado a punteros crudos
    bool isStatic;

private:
    std::string name;
    std::vector<Component*> components; // Cambiado a punteros crudos
    uint32_t UID;
    bool enabled;
};

#include "GameObject.inl"  // Incluye las implementaciones de funciones en línea de las plantillas

#endif // !__GAME_OBJECT_H__
