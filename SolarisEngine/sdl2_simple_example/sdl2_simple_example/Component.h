#pragma once
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <memory>
#include "UIDGen.h"

class GameObject;

enum class ComponentType {
    Transform,
    Camera,
    Mesh,
    Texture,
    Material,
    Unknown
};

class Component {
public:
    Component(std::shared_ptr<GameObject> containerGO, ComponentType type);
    virtual ~Component();

    virtual void Enable() = 0;  // Hacerlo puro virtual
    virtual void Disable() = 0; // Hacerlo puro virtual
    virtual void Update(double dt) = 0; // Hacerlo puro virtual
    virtual void DrawComponent() = 0; // Hacerlo puro virtual

    std::string GetName();
    void CreateNameFromType(ComponentType type);
    ComponentType GetType() const;
    std::shared_ptr<GameObject> GetContainerGO() const;
    bool IsEnabled() const;

    void CreateUID() { UID = UIDGen::GenerateRandomUint32(); }
    uint32_t GetUID() { return UID; }

protected:
    std::weak_ptr<GameObject> containerGO;
    ComponentType type;
    std::string name;
    uint32_t UID;
public:
    bool enabled;
};

#endif // !__COMPONENT_H__
