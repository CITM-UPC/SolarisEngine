#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include <glm/glm.hpp>
#include <GL/glew.h> // Incluir GLEW antes de OpenGL
#include <GL/gl.h>   // Cambiar a OpenGL (quitar GLUT)
#include <glm/gtc/type_ptr.hpp> // Para glm::value_ptr

class App;  // Declaraci�n anticipada para que el compilador conozca la clase App.

class CameraEditor {
public:
    CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    glm::mat4 getViewMatrix() const;
    void processInput(unsigned char key, bool isPressed);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseMiddle(float xoffset, float yoffset);
    void updatePosition(glm::vec3 delta);
    void Update();
    void MouseWheel(bool zoom);
    void updateCameraSpeed();
    void focusOnObject();
    void updateCameraPosition();

    void ChangeCameraSensivility(float value);
    float GetCameraSensivility();

private:
    glm::vec3 position;          // Posici�n actual de la c�mara
    glm::vec3 front;             // Direcci�n en la que mira la c�mara
    glm::vec3 up;                // Vector 'up' de la c�mara
    glm::vec3 right = glm::normalize(glm::cross(front, up));

    glm::vec3 orbitCenter;       // Punto central alrededor del cual orbitar
    float orbitRadius = 5.0f;    // Radio de la �rbita

    float yaw = -90.0f;          // �ngulo de rotaci�n alrededor del eje Y
    float pitch = 0.0f;          // �ngulo de rotaci�n alrededor del eje X
    float sensitivity = 0.1f;    // Sensibilidad del rat�n
    float baseSpeed = 0.5f;     // Velocidad base de la c�mara
    float boostedSpeedMultiplier = 2.0f; // Multiplicador de velocidad al presionar SHIFT
    float scrollBoost = 0.1f;

    bool movingForward = false;
    bool movingBackward = false;
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

    bool orbiting = false;       // Indica si la c�mara est� en modo de orbitaci�n
    bool firstOrbit = true;

    glm::vec3 objectPosition = { 0,0,0 };

    // Para conservar la posici�n y direcci�n de la c�mara antes de entrar en �rbita
    glm::vec3 savedPosition;
    glm::vec3 savedFront;
    float savedYaw;
    float savedPitch;

    float movementSpeed = 2.5f;
};

#endif // CAMERA_EDITOR_H
