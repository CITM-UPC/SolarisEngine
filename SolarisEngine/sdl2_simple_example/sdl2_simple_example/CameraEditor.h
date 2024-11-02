// CameraEditor.h
#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include <glm/glm.hpp>
#include <GL/glew.h> // Incluir GLEW antes de OpenGL
#include <GL/gl.h> // Cambiar a OpenGL (quitar GLUT)
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

    float savedYaw = 0.0f;
    float savedPitch = 0.0f;
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right = glm::normalize(glm::cross(front, up));

    glm::vec3 orbitCenter;  // Punto central alrededor del cual orbitar
    float orbitRadius = 5.0f; // Radio de la �rbita

    float yaw = -90.0f; // �ngulo de rotaci�n alrededor del eje Y
    float pitch = 0.0f; // �ngulo de rotaci�n alrededor del eje X
    float sensitivity = 0.1f; // Sensibilidad del rat�n
    float baseSpeed = 0.05f;   // Velocidad base de la c�mara
    float boostedSpeed = 0.1f; // Velocidad aumentada con Shift
    float scrollBoost = 0.1f;

    bool movingForward = false;
    bool movingBackward = false;
    bool movingLeft = false;
    bool movingRight = false;
    bool movingUp = false;
    bool movingDown = false;

    bool orbiting = false; // Indica si la c�mara est� en modo de orbitaci�n

    glm::vec3 objectPosition = { 0,0,0 };

    bool firstOrbit = true;

    glm::vec3 initialPosition;    // Guardar la posici�n inicial al iniciar la �rbita
    glm::vec3 initialFront;       // Guardar la direcci�n inicial al iniciar la �rbita
    float movementSpeed = 2.5f;
};

#endif // CAMERA_EDITOR_H
