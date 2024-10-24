#include "CameraEditor.h"
#include "App.h"  // Incluye App.h para acceder a la instancia de App




float fov = glm::radians(90.0f); // Campo de visi�n en radianes
float nearPlane = 0.01f; // Plano cercano
float farPlane = 100.0f; // Plano lejano


float aspectRatio = (float)app->WINDOW_SIZE.x / (float)app->WINDOW_SIZE.y; // Relaci�n de aspecto

glm::mat4 projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

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
    if (key == 'e') { // Cambia LCTRL por 'e' para mover hacia abajo
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

void CameraEditor::Update() {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection)); // Cargar la matriz de proyecci�n

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
