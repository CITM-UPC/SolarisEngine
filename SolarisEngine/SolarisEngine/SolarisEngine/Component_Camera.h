#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Component_Camera : public Component {
public:
    Component_Camera(GameObject* containerGO);
    ~Component_Camera();

    void Enable() override;
    void Disable() override;
    Component* Clone() const override;

    void Update(double dt) override;
    void DrawComponent() override;
    void DrawInspectorComponent() override;

    void SetFOV(float fov);
    float GetFOV() const;

    void SetAspectRatio(float aspectRatio);
    float GetAspectRatio() const;

    void SetNearPlane(float nearPlane);
    float GetNearPlane() const;

    void SetFarPlane(float farPlane);
    float GetFarPlane() const;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void SetLookAt(const glm::vec3& target);
    void SetPosition(const glm::vec3& position);
    glm::vec3 GetPosition() const;

    void SetModeFPS(bool enabled);



    


private:
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;

    float fov;          // Campo de visión
    float aspectRatio;  // Relación de aspecto
    float nearPlane;    // Plano cercano
    float farPlane;     // Plano lejano

    bool modeFPS;       // Modo FPS o vista libre

    void UpdateCameraVectors();
};
