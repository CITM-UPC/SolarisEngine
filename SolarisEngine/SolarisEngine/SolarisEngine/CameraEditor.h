#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include <glm/glm.hpp>
#include <GL/glew.h> // Incluir GLEW antes de OpenGL
#include <GL/gl.h>   // Cambiar a OpenGL (quitar GLUT)
#include <glm/gtc/type_ptr.hpp> // Para glm::value_ptr
#include "Ray.h"
#include "App.h"

class App;  // Declaración anticipada para que el compilador conozca la clase App.

class CameraEditor {
public:
    CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void processInput(unsigned char key, bool isPressed);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseMiddle(float xoffset, float yoffset);
    void updatePosition(glm::vec3 delta);
    void Update();
    void MouseWheel(bool zoom);
    void GetCameraFrustum();
    bool IsInFrustum(const glm::vec3& objectPosition);

    bool RayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax);
    
    void UpdateMousePicking(int mouseX, int mouseY, int windowWidth, int windowHeight);
    void CheckGameObject(const Ray& ray, GameObject* gameObject, GameObject*& closestObject, float& closestDistance);
    //glm::vec3 getRayFromMouse(int mouseX, int mouseY);
    //void onMouseClick(int mouseX, int mouseY);
    void updateCameraSpeed();
    void focusOnObject();
    void updateCameraPosition();

    void ChangeCameraSensivility(float value);
    float GetCameraSensivility();


    glm::vec3 GetCameraPosition();

    int drawnObjectsCount; // Contador de objetos dibujados en cada frame
    glm::vec3 position;

    float fov = 105.0f;
    float nearPlane = 0.01f;
    float farPlane = 100.0f;

    glm::vec3 front;             // Dirección en la que mira la cámara
    glm::vec3 up;                // Vector 'up' de la cámara
    glm::vec3 right = glm::normalize(glm::cross(front, up));


private:

              // Posición actual de la cámara
  

    glm::vec3 orbitCenter;       // Punto central alrededor del cual orbitar
    float orbitRadius = 5.0f;    // Radio de la órbita

    float yaw = -90.0f;          // Ángulo de rotación alrededor del eje Y
    float pitch = 0.0f;          // Ángulo de rotación alrededor del eje X
    float sensitivity = 0.1f;    // Sensibilidad del ratón
    float baseSpeed = 0.5f;     // Velocidad base de la cámara
    float boostedSpeedMultiplier = 2.0f; // Multiplicador de velocidad al presionar SHIFT
    float scrollBoost = 0.1f;

    bool movingForward = false;
    bool movingBackward = false;
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

    bool orbiting = false;       // Indica si la cámara está en modo de orbitación
    bool firstOrbit = true;

    glm::vec3 objectPosition = { 0,0,0 };

    // Para conservar la posición y dirección de la cámara antes de entrar en órbita
    glm::vec3 savedPosition;
    glm::vec3 savedFront;
    float savedYaw;
    float savedPitch;

    float movementSpeed = 2.5f;

    glm::vec4 leftPlaneFrustrum, rightPlaneFrustrum, topPlaneFrustrum, bottomPlaneFrustrum, nearPlaneFrustrum, farPlaneFrustrum;

    
};

#endif // CAMERA_EDITOR_H
