#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include "UIDGen.h"

class GameObject;

enum class ComponentType {
    Unknown = 0,
    Transform,
    Camera,
    Mesh,
    Texture,
    Material,
    BoundingBox,
    Billboard,
    ParticleSystem
    
};

class Component {
public:
    Component(GameObject* containerGO, ComponentType type); // Usar puntero crudo
    virtual ~Component();

    virtual void Enable() = 0;  // Hacerlo puro virtual
    virtual void Disable() = 0; // Hacerlo puro virtual
    virtual void Update(double dt) = 0; // Hacerlo puro virtual
    virtual void DrawComponent() = 0; // Hacerlo puro virtual
    virtual void DrawInspectorComponent() = 0; // Hacerlo puro virtual
    virtual Component* Clone() const = 0;

    std::string GetName();
    void CreateNameFromType(ComponentType type);
    ComponentType GetType() const;
    GameObject* GetContainerGO() const; // Usar puntero crudo
    bool IsEnabled() const;

    void CreateUID() { UID = UIDGen::GenerateRandomUint32(); }
    uint32_t GetUID() { return UID; }

    void SetContainer(GameObject* containerGO);


protected:
    GameObject* containerGO; // Cambiado a puntero crudo
    ComponentType type;
    std::string name;
    uint32_t UID;
public:
    bool enabled;
};

#endif // !__COMPONENT_H__
