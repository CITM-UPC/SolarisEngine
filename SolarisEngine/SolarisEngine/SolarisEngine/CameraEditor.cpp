#include "CameraEditor.h"
#include "App.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>
#include "Ray.h"

CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up), orbiting(false) {
    right = glm::normalize(glm::cross(front, up));
}

// Devuelve la matriz de vista de la cámara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 CameraEditor::getProjectionMatrix() const
{
    float fov = glm::radians(105.0f);
    float nearPlane = 0.01f;
    float farPlane = 100000.0f;

    float w = app->windowEditor->GetImGuiWindow()->scenePanel->width;
    float h = app->windowEditor->GetImGuiWindow()->scenePanel->height;

    //float aspectRatio = (float)app->WINDOW_SIZE.x / (float)app->WINDOW_SIZE.y;
    float aspectRatio = w / h;
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void CameraEditor::processInput(unsigned char key, bool isPressed) {
    if (app->inputEditor->mouseRightIsPressed) {
        switch (key) {
        case 'w': movingForward = isPressed; break;
        case 's': movingBackward = isPressed; break;
        case 'a': movingLeft = isPressed; break;
        case 'd': movingRight = isPressed; break;
        case 'q': movingUp = isPressed; break;
        case 'e': movingDown = isPressed; break;
        }
    }
    else {
        switch (key) {
        case 'w': movingForward = false; break;
        case 's': movingBackward = false; break;
        case 'a': movingLeft = false; break;
        case 'd': movingRight = false; break;
        case 'q': movingUp = false; break;
        case 'e': movingDown = false; break;
        case 'f': if (isPressed) focusOnObject(); break;
        }
    }
}

void CameraEditor::focusOnObject() {
    if (app->actualScene->GetSelectedGameObject() != nullptr) {
        glm::vec3 objectPosition = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();
        float relativeSize = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetRelativeSize();

        // Calcular la distancia requerida para ver el objeto en función de su tamaño
        float baseDistance = 1.5f;
        float distance = baseDistance * relativeSize;

        // Calcular la dirección desde la posición actual hacia el objeto
        front = glm::normalize(objectPosition - position);

        // Ajustar la posición de la cámara para estar a la distancia deseada desde el objeto en la dirección actual
        position = objectPosition - front * distance;

        // Calcular yaw y pitch para que la cámara apunte al objeto correctamente
        yaw = glm::degrees(atan2(front.z, front.x));
        pitch = glm::degrees(asin(front.y));
    }
}


void CameraEditor::updateCameraPosition() {
    float speedMultiplier = (SDL_GetModState() & KMOD_SHIFT) ? boostedSpeedMultiplier : 1.0f;
    float cameraSpeed = baseSpeed * speedMultiplier * scrollBoost;

    if (app->inputEditor->mouseRightIsPressed) {
        if (movingForward) position += cameraSpeed * front;
        if (movingBackward) position -= cameraSpeed * front;
        if (movingLeft) position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (movingRight) position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if (movingUp) position += cameraSpeed * up;
        if (movingDown) position -= cameraSpeed * up;
    }

    // Iniciar la órbita si Alt + clic izquierdo están presionados
    if (app->inputEditor->mouseLeftIsPressed && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT]) {
        if (!orbiting) {
            if (app->actualScene->GetSelectedGameObject() != nullptr) {
                orbitCenter = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();
            }
            else {
                orbitCenter = position;
            }

            glm::vec3 offset = position - orbitCenter;
            orbitRadius = glm::length(offset);

            // Calcular yaw y pitch iniciales para evitar giros bruscos al iniciar órbita
            yaw = glm::degrees(atan2(offset.z, offset.x));
            pitch = glm::degrees(asin(offset.y / orbitRadius));

            savedPosition = position;
            savedFront = front;
        }
        orbiting = true;
    }
    else if (orbiting) {
        // Al salir del modo de órbita, ajustar yaw y pitch en función de la orientación actual
        yaw = glm::degrees(atan2(front.z, front.x));
        pitch = glm::degrees(asin(front.y));

        orbiting = false;
    }
}

void CameraEditor::ChangeCameraSensivility(float value)
{
    sensitivity = value;
}

float CameraEditor::GetCameraSensivility()
{
    return sensitivity;
}

glm::vec3 CameraEditor::GetCameraPosition()
{
    return position;
}

void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (orbiting && app->actualScene->GetSelectedGameObject() != nullptr) {
        yaw += xoffset;
        pitch = glm::clamp(pitch + yoffset, -89.0f, 89.0f); 

        glm::vec3 objectPosition = app->actualScene->GetSelectedGameObject()->GetComponent<Component_Transform>()->GetPosition();
        float radius = glm::length(position - objectPosition);

        position.x = objectPosition.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        position.y = objectPosition.y + radius * sin(glm::radians(pitch));
        position.z = objectPosition.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

        front = glm::normalize(objectPosition - position);
    }
    else if (app->inputEditor->mouseRightIsPressed) {
        yaw += xoffset;
        pitch = glm::clamp(pitch - yoffset, -89.0f, 89.0f); 

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
    }

    // Debug output para seguimiento
    printf("Front.z: %f,\n", front.z);
    printf("Position.z: %f,\n", position.z);
    printf("Position.x: %f,\n", position.x);
    printf("Yaw: %f,\n", yaw);
    printf("Pitch: %f,\n", pitch);

}




void CameraEditor::processMouseMiddle(float xoffset, float yoffset) {
    float panSpeed = 0.005f;

    glm::vec3 right = glm::normalize(glm::cross(this->front, this->up));
    glm::vec3 adjustedUp = glm::normalize(glm::cross(right, this->front));

    position += -right * xoffset * panSpeed;
    position += adjustedUp * yoffset * panSpeed;
}

void CameraEditor::Update() {
    drawnObjectsCount = 0;
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(getProjectionMatrix()));

    updateCameraPosition();

    glm::mat4 view = getViewMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view));
}

void CameraEditor::MouseWheel(bool zoom) {
    const float cameraZoomSpeed = 0.05f;

    if (zoom) {
        position += cameraZoomSpeed * front;
    }
    else {
        position -= cameraZoomSpeed * front;
    }

    const float scrollSpeedIncrease = 0.1f; // Ajuste de incremento para la velocidad

    // Si "zoom" es true, significa que la rueda se ha desplazado hacia arriba (incrementa velocidad)
    if (zoom) {
        baseSpeed += scrollSpeedIncrease;
    }
    // Si "zoom" es false, significa que la rueda se ha desplazado hacia abajo (disminuye velocidad)
    else {
        baseSpeed = std::max(0.1f, baseSpeed - scrollSpeedIncrease); // Evitar que la velocidad sea negativa
    }

    printf("Velocidad de la cámara: %f\n", baseSpeed); // Para depuración
}

void CameraEditor::GetCameraFrustum()
{
    glm::mat4 viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();

    leftPlane = viewProjectionMatrix[3] + viewProjectionMatrix[0];
    rightPlane = viewProjectionMatrix[3] - viewProjectionMatrix[0];
    topPlane = viewProjectionMatrix[3] - viewProjectionMatrix[1];
    bottomPlane = viewProjectionMatrix[3] + viewProjectionMatrix[1];
    nearPlane = viewProjectionMatrix[3] + viewProjectionMatrix[2];
    farPlane = viewProjectionMatrix[3] - viewProjectionMatrix[2];

    leftPlane /= glm::length(glm::vec3(leftPlane));
    rightPlane /= glm::length(glm::vec3(rightPlane));
    topPlane /= glm::length(glm::vec3(topPlane));
    bottomPlane /= glm::length(glm::vec3(bottomPlane));
    nearPlane /= glm::length(glm::vec3(nearPlane));
    farPlane /= glm::length(glm::vec3(farPlane));

}

bool CameraEditor::IsInFrustum(const glm::vec3& objectPosition) {
    // Verificar todos los vértices de la Bounding Box de manera optimizada
    // Definir las coordenadas mínimas y máximas de la caja
    glm::vec3 min = objectPosition - glm::vec3(0.5f, 0.5f, 0.5f);  // Vértice inferior izquierdo
    glm::vec3 max = objectPosition + glm::vec3(0.5f, 0.5f, 0.5f);  // Vértice superior derecho

    // Comprobar los vértices contra el frustum
    // Definir los 8 vértices de la caja (Bounding Box)
    glm::vec3 boundingBoxVertices[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };

    // Comprobar si alguno de los vértices está dentro del frustum
    for (const auto& vertex : boundingBoxVertices) {
        if (IsInFrustum(vertex)) {
            return true;  // Si algún vértice está dentro, el objeto está en el frustum
        }
    }

    return false;  // Ningún vértice está dentro del frustum
}




