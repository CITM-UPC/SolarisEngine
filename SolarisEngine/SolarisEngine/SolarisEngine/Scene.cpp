#include "Scene.h"
#include "App.h"
#include "CameraEditor.h"
#include "Input.h"
#include <SDL2/SDL.h>

Scene::Scene()
{
}

void Scene::Update(double dt)
{

    DrawInfiniteGrid(1, 50);

    for (GameObject* gameObject : gameObjects)
    {
        if (gameObject && gameObject->IsEnabled()) {
            gameObject->Draw();
        }
    }

    if (app->inputEditor->mouseLeftIsPressed)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        /*printf("Posición del mouse: X: %d, Y: %d\n", mouseX, mouseY);*/
        UpdateMousePicking(mouseX, mouseY, 1024, 720);
    }
}

void Scene::SaveScene()
{
}

void Scene::AddGameObject(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(GameObject* gameObject) {

    if (selectedGameObject == gameObject) selectedGameObject = nullptr;

    auto it = std::remove(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it, gameObjects.end()); // Eliminar de la lista
    }
    gameObject->Delete();
    
}

void Scene::SelectGameObject(GameObject* gameObject)
{
    selectedGameObject = gameObject;
}

//void Scene::SelectGameObject(uint index)
//{
//    if(index >= 0 && index < gameObjects.size())
//        selectedGameObject = gameObjects.at(index);
//}

GameObject* Scene::GetSelectedGameObject()
{
    return selectedGameObject;
}

std::vector<GameObject*> Scene::GetGameObjectsList()
{
    return gameObjects;
}

void Scene::SetCopiedGameObject(GameObject* gameObject)
{
    this->copiedGameObject = gameObject;
}

GameObject* Scene::GetCopiedGameObject()
{
    return this->copiedGameObject;
}

void Scene::DrawGrid(float size = 10.0f, int divisions = 10)
{
    // Define el tamaño de cada celda en el grid
    float step = size / divisions;
    float halfSize = size / 2.0f;

    glPushMatrix();
    glBegin(GL_LINES);

    // Define el color del grid (por ejemplo, gris claro)
    glColor3f(0.5f, 0.5f, 0.5f);

    // Dibujar líneas en el eje X
    for (int i = 0; i <= divisions; ++i) {
        float pos = -halfSize + i * step;

        // Línea paralela al eje Z
        glVertex3f(pos, 0, -halfSize);
        glVertex3f(pos, 0, halfSize);

        // Línea paralela al eje X
        glVertex3f(-halfSize, 0, pos);
        glVertex3f(halfSize, 0, pos);
    }

    glEnd();
    glPopMatrix();
}


void Scene::DrawInfiniteGrid(float gridSpacing = 1.0f, int halfGridLines = 50)
{

    vec3 cameraPosition = app->cameraEditor->GetCameraPosition();

    // Calcula la posición del grid basado en la cámara para centrarlo
    float gridOffsetX = fmod(cameraPosition.x, gridSpacing);
    float gridOffsetZ = fmod(cameraPosition.z, gridSpacing);

    glPushMatrix();
    glBegin(GL_LINES);

    // Color del grid (gris claro)
    glColor3f(0.7f, 0.7f, 0.7f);

    // Dibujar líneas paralelas al eje Z
    for (int i = -halfGridLines; i <= halfGridLines; ++i) {
        float x = i * gridSpacing - gridOffsetX;
        glVertex3f(x, 0.0f, -halfGridLines * gridSpacing - gridOffsetZ);
        glVertex3f(x, 0.0f, halfGridLines * gridSpacing - gridOffsetZ);
    }

    // Dibujar líneas paralelas al eje X
    for (int j = -halfGridLines; j <= halfGridLines; ++j) {
        float z = j * gridSpacing - gridOffsetZ;
        glVertex3f(-halfGridLines * gridSpacing - gridOffsetX, 0.0f, z);
        glVertex3f(halfGridLines * gridSpacing - gridOffsetX, 0.0f, z);
    }

    glEnd();
    glPopMatrix();
}

bool Scene:: RayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax) {
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
    // Paso 1: Normalizar las coordenadas del mouse de [0, windowWidth] y [0, windowHeight] a [-1, 1]
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight; // La coordenada Y se invierte debido a la convención de OpenGL

    // Paso 2: Invertir la proyección y vista para convertir las coordenadas de pantalla en coordenadas del mundo
    glm::mat4 invProjection = glm::inverse(projectionMatrix);
    glm::mat4 invView = glm::inverse(viewMatrix);

    // Paso 3: Crear un vector en NDC (Normal Device Coordinates) para el punto del mouse
    glm::vec4 screenPos(x, y, -1.0f, 1.0f);  // -1.0f porque estamos transformando de coordenadas de cámara

    // Paso 4: Transformar las coordenadas del mouse desde NDC a espacio de cámara
    glm::vec4 worldPos = invProjection * screenPos;
    worldPos /= worldPos.w;  // Homogeneizar (convertir de coordenadas homogéneas a cartesiana)  

    // Paso 5: Transformar las coordenadas del mouse desde espacio de cámara a espacio de mundo
    glm::vec3 rayDir = glm::vec3(invView * worldPos);  // Dirección del rayo en el espacio de mundo

    // Paso 6: Crear el rayo desde la cámara
    Ray ray(cameraPosition, rayDir);

    printf("rayDir: x = %f, y = %f, z = %f\n", rayDir.x, rayDir.y, rayDir.z);

    return ray;
}

void Scene::UpdateMousePicking(int mouseX, int mouseY, int windowWidth, int windowHeight) {
    // Crear el rayo desde el mouse
    Ray ray = GetMouseRay(mouseX, mouseY, windowWidth, windowHeight, app->cameraEditor->getProjectionMatrix(), app->cameraEditor->getViewMatrix(), app->cameraEditor->position);

    GameObject* closestObject = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (GameObject* gameObject : gameObjects) {
        if (auto meshComponent = gameObject->GetComponent<Component_Mesh>()) {
            auto [boxMin, boxMax] = meshComponent->GetBoundingBoxInWorldSpace();

            // Verificar intersección
            if (RayIntersectsAABB(ray, boxMin, boxMax)) {
                // Calcular la distancia desde la cámara
                float distance = glm::length(boxMin - ray.origin);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestObject = gameObject;
                }
            }
        }
    }

    // Seleccionar el objeto más cercano
    /*SelectGameObject(closestObject);*/

    DrawRay(ray, 100.0f);
}

void Scene::DrawRay(const Ray& ray, float length) {
    glm::vec3 endPoint = ray.origin + ray.direction * length;

    glEnable(GL_DEPTH_TEST);

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






