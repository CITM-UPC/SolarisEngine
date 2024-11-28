#pragma once
#include "Component.h"
#include "Component_Transform.h"
#include "App.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class BillboardType {
    SCREEN_ALIGNED,
    WORLD_ALIGNED,
    AXIS_ALIGNED
};
enum class AxisAlignment {
    X_AXIS,
    Y_AXIS,
    Z_AXIS,
    CUSTOM_AXIS
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

    void SetAxisAlignment(AxisAlignment newAxisAlignment);
    void SetCustomAxis(const glm::vec3& newAxis);
    glm::vec3 GetAxis() const;

   


private:
    BillboardType type;
    AxisAlignment axisAlignment = AxisAlignment::Y_AXIS; // Eje predeterminado
    glm::vec3 customAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Eje personalizado

    glm::mat4 CalculateBillboardMatrix(const glm::mat4& viewMatrix);

    glm::vec3 GetAlignedAxis() const;
    void UpdateAxisFromAlignment();

 
};

