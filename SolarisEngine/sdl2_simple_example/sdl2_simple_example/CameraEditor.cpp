#include "CameraEditor.h"
#include "App.h"  // Incluye App.h para acceder a la instancia de App
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>




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

void CameraEditor::processInput(unsigned char key, bool isPressed) {
    if (app->inputEditor->mouseRightIsPressed) { // Solo procesar si el bot�n derecho est� presionado
        switch (key) {
        case 'w':
            movingForward = isPressed;  // Activar/desactivar el movimiento hacia adelante
            break;
        case 's':
            movingBackward = isPressed;  // Activar/desactivar el movimiento hacia atr�s
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
    else {
        // Si el mouse no est� presionado, no permitimos el movimiento
        if (isPressed) {
            // Opci�n: podr�as limpiar los movimientos si el mouse no est� presionado
            switch (key) {
            case 'w':
            case 's':
            case 'a':
            case 'd':
            case 'q':
            case 'e':
                // Podr�as hacer que no se active el movimiento en absoluto o manejar de otra manera
                break;
            }
        }
    }
}
void CameraEditor::updateCameraPosition() {
    // Determina la velocidad de la c�mara en funci�n del estado de Shift
    float cameraSpeed = (SDL_GetModState() & KMOD_SHIFT) ? boostedSpeed : baseSpeed;

    // Solo mueve la c�mara si el bot�n derecho est� presionado
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
    else {
        // Aqu� puedes resetear los estados de movimiento si lo deseas
        movingForward = false;
        movingBackward = false;
        movingLeft = false;
        movingRight = false;
        movingUp = false;
        movingDown = false;
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

void CameraEditor::processMouseMiddle(float xoffset, float yoffset) {
    // Sensibilidad para el desplazamiento
    float panSpeed = 0.005f;

    // Recalcula los vectores `right` y `up` con la orientaci�n actual de la c�mara
    glm::vec3 right = glm::normalize(glm::cross(this->front, this->up));
    glm::vec3 adjustedUp = glm::normalize(glm::cross(right, this->front));

    // Ajusta la posici�n de la c�mara en el plano de la c�mara utilizando `right` y `adjustedUp`
    position += -right * xoffset * panSpeed;       // Desplazamiento horizontal
    position += adjustedUp * yoffset * panSpeed;    // Desplazamiento vertical en el plano de la c�mara
}

void CameraEditor::Update() {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection)); // Cargar la matriz de proyecci�n

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
