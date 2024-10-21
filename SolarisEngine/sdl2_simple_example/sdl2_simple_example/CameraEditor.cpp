#include "CameraEditor.h"
#include <SDL2/SDL_scancode.h>

// Constructor de la cámara
CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up) {}

// Devuelve la matriz de vista de la cámara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

// Procesa las entradas del teclado para mover la cámara
void CameraEditor::processInput(unsigned char key) {
    const float cameraSpeed = 0.05f; // Velocidad de movimiento de la cámara
    if (key == 'w') {
        position += cameraSpeed * front; // Mover hacia adelante
    }
    if (key == 's') {
        position -= cameraSpeed * front; // Mover hacia atrás
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

void CameraEditor::updatePosition(glm::vec3 delta) {
    position += delta; // Método para actualizar la posición
}
