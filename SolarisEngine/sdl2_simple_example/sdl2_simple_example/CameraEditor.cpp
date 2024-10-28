#include "CameraEditor.h"
#include "App.h"  // Incluye App.h para acceder a la instancia de App
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>




float fov = glm::radians(90.0f); // Campo de visión en radianes
float nearPlane = 0.01f; // Plano cercano
float farPlane = 100.0f; // Plano lejano


float aspectRatio = (float)app->WINDOW_SIZE.x / (float)app->WINDOW_SIZE.y; // Relación de aspecto

glm::mat4 projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

// Constructor de la cámara
CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up) {}

// Devuelve la matriz de vista de la cámara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void CameraEditor::processInput(unsigned char key, bool isPressed) {
    switch (key) {
    case 'w':
        movingForward = isPressed;
        break;
    case 's':
        movingBackward = isPressed;
        break;
    case 'a':
        movingLeft = isPressed;
        break;
    case 'd':
        movingRight = isPressed;
        break;
    case 'q':
        movingUp = isPressed;
        break;
    case 'e':
        movingDown = isPressed;
        break;
    }
}
void CameraEditor::updateCameraPosition() {
    // Determina la velocidad de la cámara en función del estado de Shift
    float cameraSpeed = (SDL_GetModState() & KMOD_SHIFT) ? boostedSpeed : baseSpeed;

    // Mueve la cámara en función de las teclas activas
    if (movingForward) {
        position += cameraSpeed * front;
    }
    if (movingBackward) {
        position -= cameraSpeed * front;
    }
    if (movingLeft) {
        position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }
    if (movingRight) {
        position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }
    if (movingUp) {
        position += cameraSpeed * up;
    }
    if (movingDown) {
        position -= cameraSpeed * up;
    }
}

    


void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; // Ajustar rotación alrededor del eje Y
    pitch -= yoffset; // Ajustar rotación alrededor del eje X

    // Limitar el pitch para evitar problemas de "gimbal lock"
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calcular la nueva dirección de la cámara
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
}

void CameraEditor::Update() {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection)); // Cargar la matriz de proyección

    updateCameraPosition();


    glm::mat4 view = getViewMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view)); // Cargar la matriz de vista
}

void CameraEditor::MouseWheel(bool zoom) {
    const float cameraSpeed = 0.05f;
    if (zoom) {
        position += cameraSpeed * front;
    }
    else {
        position -= cameraSpeed * front;
    }
}
