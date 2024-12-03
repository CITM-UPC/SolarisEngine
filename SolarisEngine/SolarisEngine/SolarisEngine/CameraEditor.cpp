#include "CameraEditor.h"
#include "App.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_keyboard.h>
#include "Ray.h"
#include "Debug.h"
#include <SDL2/SDL.h>
#include "Scene.h"

CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up), orbiting(false) {
    right = glm::normalize(glm::cross(front, up));
}

// Devuelve la matriz de vista de la c�mara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 CameraEditor::getProjectionMatrix() const
{
    float fov = glm::radians(this->fov);
    float nearPlane = 0.01f;
    float farPlane = 100.0f;

    float w = app->windowEditor->GetImGuiWindow()->scenePanel->width;
    float h = app->windowEditor->GetImGuiWindow()->scenePanel->height;

    //float aspectRatio = (float)app->WINDOW_SIZE.x / (float)app->WINDOW_SIZE.y;
    if (h == 0.0f) h = 1.0f;
    float aspectRatio = w / h;
    //Debug::Log("Aspect ratio: ", aspectRatio);
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

        // Calcular la distancia requerida para ver el objeto en funci�n de su tama�o
        float baseDistance = 1.5f;
        float distance = baseDistance * relativeSize;

        // Calcular la direcci�n desde la posici�n actual hacia el objeto
        front = glm::normalize(objectPosition - position);

        // Ajustar la posici�n de la c�mara para estar a la distancia deseada desde el objeto en la direcci�n actual
        position = objectPosition - front * distance;

        // Calcular yaw y pitch para que la c�mara apunte al objeto correctamente
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

    // Iniciar la �rbita si Alt + clic izquierdo est�n presionados
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

            // Calcular yaw y pitch iniciales para evitar giros bruscos al iniciar �rbita
            yaw = glm::degrees(atan2(offset.z, offset.x));
            pitch = glm::degrees(asin(offset.y / orbitRadius));

            savedPosition = position;
            savedFront = front;
        }
        orbiting = true;
    }
    else if (orbiting) {
        // Al salir del modo de �rbita, ajustar yaw y pitch en funci�n de la orientaci�n actual
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
    /*printf("Front.z: %f,\n", front.z);
    printf("Position.z: %f,\n", position.z);
    printf("Position.x: %f,\n", position.x);
    printf("Yaw: %f,\n", yaw);
    printf("Pitch: %f,\n", pitch);*/

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

    if (app->inputEditor->mouseLeftIsPressed && app->actualScene->isScenePicked && !app->inputEditor->isCameraMoving)
    {
        
    }
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

    printf("Velocidad de la c�mara: %f\n", baseSpeed); // Para depuraci�n
}

void CameraEditor::GetCameraFrustum()
{
    glm::mat4 viewProjectionMatrix = getProjectionMatrix() * getViewMatrix();

    leftPlaneFrustrum = viewProjectionMatrix[3] + viewProjectionMatrix[0];
    rightPlaneFrustrum = viewProjectionMatrix[3] - viewProjectionMatrix[0];
    topPlaneFrustrum = viewProjectionMatrix[3] - viewProjectionMatrix[1];
    bottomPlaneFrustrum = viewProjectionMatrix[3] + viewProjectionMatrix[1];
    nearPlaneFrustrum = viewProjectionMatrix[3] + viewProjectionMatrix[2];
    farPlaneFrustrum = viewProjectionMatrix[3] - viewProjectionMatrix[2];

    leftPlaneFrustrum /= glm::length(glm::vec3(leftPlaneFrustrum));
    rightPlaneFrustrum /= glm::length(glm::vec3(rightPlaneFrustrum));
    topPlaneFrustrum /= glm::length(glm::vec3(topPlaneFrustrum));
    bottomPlaneFrustrum /= glm::length(glm::vec3(bottomPlaneFrustrum));
    nearPlane /= glm::length(glm::vec3(nearPlaneFrustrum));
    farPlane /= glm::length(glm::vec3(farPlaneFrustrum));

}

bool CameraEditor::IsInFrustum(const glm::vec3& objectPosition) {
    // Verificar todos los v�rtices de la Bounding Box de manera optimizada
    // Definir las coordenadas m�nimas y m�ximas de la caja
    glm::vec3 min = objectPosition - glm::vec3(0.5f, 0.5f, 0.5f);  // V�rtice inferior izquierdo
    glm::vec3 max = objectPosition + glm::vec3(0.5f, 0.5f, 0.5f);  // V�rtice superior derecho

    // Comprobar los v�rtices contra el frustum
    // Definir los 8 v�rtices de la caja (Bounding Box)
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

    // Comprobar si alguno de los v�rtices est� dentro del frustum
    for (const auto& vertex : boundingBoxVertices) {
        if (IsInFrustum(vertex)) {
            return true;  // Si alg�n v�rtice est� dentro, el objeto est� en el frustum
        }
    }

    return false;  // Ning�n v�rtice est� dentro del frustum
}



bool CameraEditor::RayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    float tMin = (boxMin.x - ray.origin.x) / ray.direction.x;
    float tMax = (boxMax.x - ray.origin.x) / ray.direction.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (boxMin.y - ray.origin.y) / ray.direction.y;
    float tyMax = (boxMax.y - ray.origin.y) / ray.direction.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (boxMin.z - ray.origin.z) / ray.direction.z;
    float tzMax = (boxMax.z - ray.origin.z) / ray.direction.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

Ray GetMouseRay(int mouseX, int mouseY, int windowWidth, int windowHeight, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition) {
    // Normalizar coordenadas del mouse a [-1, 1]


    /*No mirar*/
    mouseX -= app->windowEditor->GetImGuiWindow()->scenePanel->scenePanelPos.x;
    mouseX -= 6;
    mouseY -= app->windowEditor->GetImGuiWindow()->scenePanel->scenePanelPos.y;
    mouseY -= 25;

    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;



    // Invertir las matrices de proyecci�n y vista
    glm::mat4 invProjection = glm::inverse(projectionMatrix);
    glm::mat4 invView = glm::inverse(viewMatrix);

    // Near y Far en espacio NDC
    glm::vec4 screenPosNear(x, y, -1.0f, 1.0f);
    glm::vec4 screenPosFar(x, y, 1.0f, 1.0f);

    // Transformar a espacio mundial
    glm::vec4 nearPointWorld = invView * (invProjection * screenPosNear);
    glm::vec4 farPointWorld = invView * (invProjection * screenPosFar);

    // Homogeneizar
    nearPointWorld /= nearPointWorld.w;
    farPointWorld /= farPointWorld.w;

    // Calcular direcci�n del rayo
    glm::vec3 rayDir = glm::normalize(glm::vec3(farPointWorld) - glm::vec3(nearPointWorld));

    // Crear el rayo
    return Ray(cameraPosition, rayDir);
}

void CameraEditor::UpdateMousePicking(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    // Crear el rayo desde el mouse
    Ray ray = GetMouseRay(mouseX, mouseY, windowWidth, windowHeight, app->cameraEditor->getProjectionMatrix(), app->cameraEditor->getViewMatrix(), app->cameraEditor->position);

    GameObject* closestObject = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    // Recursivamente buscar en los objetos de la escena y sus hijos
    for (GameObject* gameObject : app->actualScene->GetGameObjects()) {
        CheckGameObject(ray, gameObject, closestObject, closestDistance);
    }

    // Seleccionar el objeto m�s cercano
    
    app->actualScene->SelectGameObjectInScene(closestObject);

     /*DrawRay(ray, 100.0f);*/
}

void CameraEditor::CheckGameObject(const Ray& ray, GameObject* gameObject, GameObject*& closestObject, float& closestDistance) {
    if (auto meshComponent = gameObject->GetComponent<Component_Mesh>()) {
        auto [boxMin, boxMax] = meshComponent->GetBoundingBoxInWorldSpace();

        // Verificar intersecci�n
        if (RayIntersectsAABB(ray, boxMin, boxMax)) {
            // Calcular la distancia desde la c�mara
            float distance = glm::length(boxMin - ray.origin);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestObject = gameObject;
            }
        }
    }

    // Revisar los objetos hijos del GameObject
    for (GameObject* child : gameObject->GetChildren()) {
        CheckGameObject(ray, child, closestObject, closestDistance); // Llamada recursiva
    }
}

void CameraEditor::DrawRay(const Ray& ray, float length) {
    glm::vec3 endPoint = ray.origin + ray.direction * length;



    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f); // Rojo para el rayo

    // Dibuja el rayo
    glBegin(GL_LINES);
    glVertex3f(ray.origin.x, ray.origin.y, ray.origin.z);
    glVertex3f(endPoint.x, endPoint.y, endPoint.z);
    glEnd();

    // Dibuja el punto de origen
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(ray.origin.x, ray.origin.y, ray.origin.z);
    glVertex3f(endPoint.x, endPoint.y, endPoint.z);
    glEnd();

    glPopMatrix();
}






