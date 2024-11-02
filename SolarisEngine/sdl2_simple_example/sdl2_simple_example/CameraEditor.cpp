// CameraEditor.cpp
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
    : position(position), front(front), up(up), orbiting(false) {
    right = glm::normalize(glm::cross(front, up)); // Inicializar right
}

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
    else
    {
        switch (key) {
        case 'w':
            movingForward = false;
            break;
        case 's':
            movingBackward = false;
            break;
        case 'a':
            movingLeft = false;
            break;
        case 'd':
            movingRight = false;
            break;
        case 'q':
            movingUp = false;
            break;
        case 'e':
            movingDown = false;
            break;
        case 'f':
            if (isPressed) { // Se activa solo cuando se presiona F
                focusOnObject();
            }
            break;
        }
    }
    //if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
    //    float distance = 5.0f;
    //    glm::vec3 objectPosition = app->windowEditor->GetImGuiWindow()->inspectorPanel->selectedGameObject->GetComponent<Component_Transform>()->GetPosition();
    //    glm::vec3 objectPosition = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();

    //     Actualiza la posici�n de la c�mara
    //    position = objectPosition + glm::vec3(0.0f, 0.0f, -distance);

    //     Calcula el nuevo 'front' para que apunte al objeto
    //    
    //    front = glm::normalize(objectPosition - position);

    //     Actualiza yaw y pitch basados en la nueva direcci�n front
    //    yaw = atan2(front.z, front.x); // Aseg�rate de que el orden es correcto
    //    pitch = asin(front.y);
    //    

    //}


}

void CameraEditor::focusOnObject() {
    if (app->actualScene->GetSelectedGameObject() != nullptr)
    {
        glm::vec3 objectPosition = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();
        float relativeSize = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetRelativeSize();

        float baseDistance = 1.0f;
        float distance = baseDistance * relativeSize;

        glm::vec3 offset(0.0f, 1.0f, -distance);
        position = objectPosition + offset;
        front = glm::normalize(objectPosition - position);

        yaw = glm::degrees(atan2(front.z, front.x));
        pitch = glm::degrees(asin(front.y));
    }
    
}




// Agregar variables para almacenar yaw y pitch al empezar a orbitar
float savedYaw = 0.0f;
float savedPitch = 0.0f;

void CameraEditor::updateCameraPosition() {
    float cameraSpeed = ((SDL_GetModState() & KMOD_SHIFT) ? boostedSpeed : baseSpeed) * scrollBoost * 10;

    // Movimiento en modo libre (con el bot�n derecho)
    if (app->inputEditor->mouseRightIsPressed) {
        if (movingForward) position += cameraSpeed * front;
        if (movingBackward) position -= cameraSpeed * front;
        if (movingLeft) position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (movingRight) position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (movingUp) position += cameraSpeed * up;
        if (movingDown) position -= cameraSpeed * up;
    }

    // Iniciar la �rbita si Alt + clic izquierdo est�n presionados
    if (app->inputEditor->mouseLefttIsPressed && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT]) {
        if (!orbiting) {
            firstOrbit = true; // Reiniciar la �rbita solo al entrar
        }
        orbiting = true;
    }
    else {
        orbiting = false;
    }
    
}


void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (orbiting) {
        if (app->actualScene->GetSelectedGameObject() != nullptr) { // Ajustar yaw y pitch durante la orbitaci�n

            yaw += xoffset;
            pitch -= yoffset;

            // Limitar el pitch para evitar problemas de "gimbal lock"
            pitch = glm::clamp(pitch, -89.0f, 89.0f); // Usar glm::clamp para mayor claridad

            // Recalcular la posici�n de la c�mara alrededor del objeto
            glm::vec3 objectPosition = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();
            float radius = glm::length(position - objectPosition);  // Mantener una distancia constante

            // Calcular la nueva posici�n de la c�mara en funci�n de yaw y pitch
            position.x = objectPosition.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            position.y = objectPosition.y + radius * sin(glm::radians(pitch));
            position.z = objectPosition.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(-yaw));

            // Asegurar que la c�mara siempre apunte al objeto
            front = glm::normalize(objectPosition - position);
        }
    }
    else if (app->inputEditor->mouseRightIsPressed) {
        // Movimiento de c�mara normal si no est� orbitando
        yaw += xoffset;
        pitch -= yoffset;

        // Limitar el pitch para evitar problemas de "gimbal lock"
        pitch = glm::clamp(pitch, -89.0f, 89.0f);

        // Recalcular front basado en yaw y pitch
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
    }

    // Debug output
    printf("Front.z: %f,\n", front.z); // Actualizar esto seg�n sea necesario
    printf("Position.z: %f,\n", position.z); // Actualizar esto seg�n sea necesario
    printf("Position.x: %f,\n", position.x); // Actualizar esto seg�n sea necesario
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
    const float scrollSpeedIncrease = 0.01f;  // Incremento por cada scroll
    const float cameraZoomSpeed = 0.05f;

    if (movingForward || movingBackward || movingLeft || movingRight || movingUp || movingDown) {
        // Si se est� moviendo, ajusta la velocidad de desplazamiento
        if (zoom) {
            scrollBoost += scrollSpeedIncrease;
        }
        else {
            scrollBoost = std::max(0.2f, scrollBoost - scrollSpeedIncrease);  // Evitar valores menores a 1
        }
    }
    else {
        // Si no hay movimiento, realiza un zoom ajustando directamente la posici�n
        if (zoom) {
            position += cameraZoomSpeed * front;
        }
        else {
            position -= cameraZoomSpeed * front;
        }
    }
}
