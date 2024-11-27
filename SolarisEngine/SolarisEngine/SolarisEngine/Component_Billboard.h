#pragma once
#include "Component.h"
#include "Component_Transform.h"
#include "App.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class BillboardType {
    SCREEN_ALIGNED,
    WORLD_ALIGNED
};

class Component_Billboard : public Component {
public:
    Component_Billboard(GameObject* containerGO, BillboardType type = BillboardType::SCREEN_ALIGNED);
    ~Component_Billboard() override;

    void Update(double dt) override;
    void DrawInspectorComponent() override;
    Component* Clone() const override;

    void DrawComponent() override;

    void Enable() override;

    void Disable() override;

    void SetType(BillboardType newType);
    BillboardType GetType() const;

private:
    BillboardType type;
    glm::mat4 CalculateBillboardMatrix(const glm::mat4& viewMatrix);
};

