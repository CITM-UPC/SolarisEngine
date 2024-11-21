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

    template <typename TComponent>
    TComponent* AddComponent(TComponent* component);

    void RemoveComponent(ComponentType type);

    std::vector<Component*> GetComponents();

    // Get/Set
    bool IsEnabled() const;
    void Enable();
    void Disable();
    void SetEnable(bool enable);
    void Delete(); // Método para eliminar


    std::string GetName() const;
    void SetName(const std::string& name);

    bool IsStatic() const;
    void SetStatic(bool isStatic);

    void CreateUID();
    GameObject* Duplicate() const;
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    std::vector<GameObject*> GetChildren();
    void SetParent(GameObject* parent);
    void RemoveParent();
    uint32_t GetUID() { return UID; }

private:
    // Constructor privado para que solo pueda usarse a través de Create()
    GameObject(const std::string& name = "gameObject");

public:
    GameObject* parent = nullptr; // Cambiado a puntero crudo
    std::vector<GameObject*> children; // Cambiado a punteros crudos
    bool isStatic;
    bool enabled;

    bool isExpanded = false;

private:
    std::string name;
    std::vector<Component*> components; // Cambiado a punteros crudos
    uint32_t UID;
  
};

#include "GameObject.inl"  // Incluye las implementaciones de funciones en línea de las plantillas

#endif // !__GAME_OBJECT_H__
