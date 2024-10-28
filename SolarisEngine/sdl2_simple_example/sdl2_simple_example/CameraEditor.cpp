// CameraEditor.cpp
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
    : position(position), front(front), up(up), orbiting(false) {
    right = glm::normalize(glm::cross(front, up)); // Inicializar right
}

// Devuelve la matriz de vista de la cámara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void CameraEditor::processInput(unsigned char key, bool isPressed) {
    if (app->inputEditor->mouseRightIsPressed) { // Solo procesar si el botón derecho está presionado
        switch (key) {
        case 'w':
            movingForward = isPressed;  // Activar/desactivar el movimiento hacia adelante
            break;
        case 's':
            movingBackward = isPressed;  // Activar/desactivar el movimiento hacia atrás
            break;
        case 'a':
            movingLeft = isPressed;      // Activar/desactivar el movimiento hacia la izquierda
            break;
        case 'd':
            movingRight = isPressed;     // Activar/desactivar el movimiento hacia la derecha
            break;
        case 'q':
            movingUp = isPressed;        // Activar/desactivar el movimiento hacia arriba
            break;
        case 'e':
            movingDown = isPressed;      // Activar/desactivar el movimiento hacia abajo
            break;
        }
    }
}

void CameraEditor::updateCameraPosition() {
    // Determina la velocidad de la cámara con el estado de Shift y scroll
    float cameraSpeed = ((SDL_GetModState() & KMOD_SHIFT) ? boostedSpeed : baseSpeed) * scrollBoost * 10;

    // Solo mueve la cámara si el botón derecho está presionado
    if (app->inputEditor->mouseRightIsPressed) {
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

    // Lógica de orbitación al hacer clic izquierdo y presionar Alt
    if (app->inputEditor->mouseLefttIsPressed && (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT])) {
        orbiting = true;
    }
    else {
        orbiting = false;
    }

    if (orbiting) {
        // Calcular el punto central de la órbita
        orbitCenter = position + front; // Puedes ajustar esto para definir el centro de la órbita más específicamente

        // Calcular el radio de la órbita como la distancia entre la cámara y el punto central
        orbitRadius = glm::length(position - orbitCenter);

        // Calcular la nueva posición de la cámara en órbita
        position.x = orbitCenter.x + orbitRadius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        position.y = orbitCenter.y + orbitRadius * sin(glm::radians(pitch));
        position.z = orbitCenter.z + orbitRadius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    }
}

void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    if (!app->inputEditor->mouseLefttIsPressed) {
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
}

void CameraEditor::processMouseMiddle(float xoffset, float yoffset) {
    // Sensibilidad para el desplazamiento
    float panSpeed = 0.005f;

    // Recalcula los vectores `right` y `up` con la orientación actual de la cámara
    glm::vec3 right = glm::normalize(glm::cross(this->front, this->up));
    glm::vec3 adjustedUp = glm::normalize(glm::cross(right, this->front));

    // Ajusta la posición de la cámara en el plano de la cámara utilizando `right` y `adjustedUp`
    position += -right * xoffset * panSpeed;       // Desplazamiento horizontal
    position += adjustedUp * yoffset * panSpeed;    // Desplazamiento vertical en el plano de la cámara
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
    const float scrollSpeedIncrease = 0.01f;  // Incremento por cada scroll
    const float cameraZoomSpeed = 0.05f;

    if (movingForward || movingBackward || movingLeft || movingRight || movingUp || movingDown) {
        // Si se está moviendo, ajusta la velocidad de desplazamiento
        if (zoom) {
            scrollBoost += scrollSpeedIncrease;
        }
        else {
            scrollBoost = std::max(0.2f, scrollBoost - scrollSpeedIncrease);  // Evitar valores menores a 1
        }
    }
    else {
        // Si no hay movimiento, realiza un zoom ajustando directamente la posición
        if (zoom) {
            position += cameraZoomSpeed * front;
        }
        else {
            position -= cameraZoomSpeed * front;
        }
    }
}
