#include "CameraEditor.h"
#include <SDL2/SDL_scancode.h>

// Constructor de la c�mara
CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up) {}

// Devuelve la matriz de vista de la c�mara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

// Procesa las entradas del teclado para mover la c�mara
void CameraEditor::processInput(unsigned char key) {
    const float cameraSpeed = 0.05f; // Velocidad de movimiento de la c�mara
    if (key == 'w') {
        position += cameraSpeed * front; // Mover hacia adelante
    }
    if (key == 's') {
        position -= cameraSpeed * front; // Mover hacia atr�s
    }
    if (key == 'a') {
        position -= glm::normalize(glm::cross(front, up)) * cameraSpeed; // Mover a la izquierda
    }
    if (key == 'd') {
        position += glm::normalize(glm::cross(front, up)) * cameraSpeed; // Mover a la derecha
    }
    if (key == 'q') {
        position += cameraSpeed * up; // Mover hacia arriba
    }
    if (key == SDL_SCANCODE_LCTRL) {
        position -= cameraSpeed * up; // Mover hacia abajo
    }
}

void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; // Ajustar rotaci�n alrededor del eje Y
    pitch -= yoffset; // Ajustar rotaci�n alrededor del eje X

    // Limitar el pitch para evitar problemas de "gimbal lock"
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calcular la nueva direcci�n de la c�mara
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
}

void CameraEditor::updatePosition(glm::vec3 delta) {
    position += delta; // M�todo para actualizar la posici�n
}
