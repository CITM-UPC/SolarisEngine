#include "Component_Camera.h"
#include "App.h"
#include <iostream>

Component_Camera::Component_Camera(GameObject* containerGO)
    : Component(containerGO, ComponentType::Camera),
    position(0.0f, 0.0f, 0.0f),
    forward(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    right(1.0f, 0.0f, 0.0f),
    fov(45.0f),
    aspectRatio(16.0f / 9.0f),
    nearPlane(0.1f),
    farPlane(1000.0f),
    modeFPS(true) {
    UpdateCameraVectors();
}

Component_Camera::~Component_Camera() {}

void Component_Camera::Enable() {
    enabled = true;
}

void Component_Camera::Disable() {
    enabled = false;
}

Component* Component_Camera::Clone() const
{
    return new Component_Camera(*this);
}

void Component_Camera::Update(double dt) {
    // Aquí puedes manejar el movimiento de la cámara si está en modo FPS
}
void Component_Camera::DrawComponent() {
    if (!enabled) {
        return;
    }

    // Distancia fija para el plano lejano visual
    float farPlaneVisual = 2.5f;

    // Near plane corners en espacio cámara
    glm::vec3 nearCorners[4] = {
        { -nearPlane * tan(glm::radians(fov) * 0.5f) * aspectRatio, -nearPlane * tan(glm::radians(fov) * 0.5f), -nearPlane },
        {  nearPlane * tan(glm::radians(fov) * 0.5f) * aspectRatio, -nearPlane * tan(glm::radians(fov) * 0.5f), -nearPlane },
        { -nearPlane * tan(glm::radians(fov) * 0.5f) * aspectRatio,  nearPlane * tan(glm::radians(fov) * 0.5f), -nearPlane },
        {  nearPlane * tan(glm::radians(fov) * 0.5f) * aspectRatio,  nearPlane * tan(glm::radians(fov) * 0.5f), -nearPlane },
    };

    // Far plane corners visual en espacio cámara
    glm::vec3 farCorners[4] = {
        { -farPlaneVisual * tan(glm::radians(fov) * 0.5f) * aspectRatio, -farPlaneVisual * tan(glm::radians(fov) * 0.5f), -farPlaneVisual },
        {  farPlaneVisual * tan(glm::radians(fov) * 0.5f) * aspectRatio, -farPlaneVisual * tan(glm::radians(fov) * 0.5f), -farPlaneVisual },
        { -farPlaneVisual * tan(glm::radians(fov) * 0.5f) * aspectRatio,  farPlaneVisual * tan(glm::radians(fov) * 0.5f), -farPlaneVisual },
        {  farPlaneVisual * tan(glm::radians(fov) * 0.5f) * aspectRatio,  farPlaneVisual * tan(glm::radians(fov) * 0.5f), -farPlaneVisual },
    };

    // Transformar las esquinas al espacio mundial
    glm::vec3 nearCornersWorld[4];
    glm::vec3 farCornersWorld[4];
    for (int i = 0; i < 4; ++i) {
        nearCornersWorld[i] = position + right * nearCorners[i].x + up * nearCorners[i].y + forward * nearCorners[i].z;
        farCornersWorld[i] = position + right * farCorners[i].x + up * farCorners[i].y + forward * farCorners[i].z;
    }

    // Dibujar líneas
    glDisable(GL_DEPTH_TEST); // Opcional: para evitar problemas de profundidad
    glBegin(GL_LINES);

    // Near plane
    glColor3f(0.0f, 1.0f, 0.0f); // Verde para el plano cercano
    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&nearCornersWorld[i].x);
        glVertex3fv(&nearCornersWorld[(i + 1) % 4].x);
    }

    // Far plane visual
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para el plano lejano visual
    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&farCornersWorld[i].x);
        glVertex3fv(&farCornersWorld[(i + 1) % 4].x);
    }

    // Conectar near y far planes
    glColor3f(0.0f, 0.0f, 1.0f); // Azul para las líneas que conectan
    for (int i = 0; i < 4; ++i) {
        glVertex3fv(&nearCornersWorld[i].x);
        glVertex3fv(&farCornersWorld[i].x);
    }

    glEnd();
    glEnable(GL_DEPTH_TEST);
}
void Component_Camera::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader(u8"\ue0b0 Camera")) {
        if (ImGui::Checkbox("Enable Camera", &enabled)) {
            enabled ? Enable() : Disable();
        }

        ImGui::DragFloat("FOV", &fov, 1.0f, 1.0f, 120.0f);
        ImGui::DragFloat("Aspect Ratio", &aspectRatio, 0.01f, 0.1f, 4.0f);
        ImGui::DragFloat("Near Plane", &nearPlane, 0.01f, 0.01f, farPlane);
        ImGui::DragFloat("Far Plane", &farPlane, 1.0f, nearPlane + 0.1f, 10000.0f);

        if (ImGui::Checkbox("FPS Mode", &modeFPS)) {
            SetModeFPS(modeFPS);
        }
    }
}

void Component_Camera::SetFOV(float newFOV) {
    fov = newFOV;
}

float Component_Camera::GetFOV() const {
    return fov;
}

void Component_Camera::SetAspectRatio(float newAspectRatio) {
    aspectRatio = newAspectRatio;
}

float Component_Camera::GetAspectRatio() const {
    return aspectRatio;
}

void Component_Camera::SetNearPlane(float newNearPlane) {
    nearPlane = newNearPlane;
}

float Component_Camera::GetNearPlane() const {
    return nearPlane;
}

void Component_Camera::SetFarPlane(float newFarPlane) {
    farPlane = newFarPlane;
}

float Component_Camera::GetFarPlane() const {
    return farPlane;
}

glm::mat4 Component_Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Component_Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Component_Camera::SetLookAt(const glm::vec3& target) {
    forward = glm::normalize(target - position);
    UpdateCameraVectors();
}

void Component_Camera::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

glm::vec3 Component_Camera::GetPosition() const {
    return position;
}

void Component_Camera::SetModeFPS(bool enabled) {
    modeFPS = enabled;
}

void Component_Camera::UpdateCameraVectors() {
    forward = glm::normalize(forward);
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::cross(right, forward);
}
