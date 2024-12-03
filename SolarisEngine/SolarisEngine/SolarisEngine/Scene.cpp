#include "Scene.h"
#include "App.h"
#include "CameraEditor.h"
#include "Input.h"
#include <SDL2/SDL.h>
#include "Debug.h"

Scene::Scene()
{
}

void Scene::Update(double dt)
{

    UpdateGameObjectsRecursively(gameObjects, dt);

    
}

void Scene::Draw()
{
    DrawInfiniteGrid(1, 50);
    DrawGameObjectsRecursively(gameObjects);
}

void Scene::SaveScene()
{
}

void Scene::AddGameObject(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::RemoveRootGameObject(GameObject* gameObject)
{
    if (selectedGameObject == gameObject) selectedGameObject = nullptr;
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
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
void Scene::SelectGameObjectInScene(GameObject* gameObject)
{
    if (gameObject != nullptr)
    {
        if (IsChildOfSelected(gameObject)) {
            selectedGameObject = gameObject; // Selecciona el hijo si el padre está seleccionado.
        }
        else {
            selectedGameObject = GetRootParent(gameObject); // Selecciona el ancestro raíz si no.
        }
    }
    
}

// Verifica si el objeto es descendiente del seleccionado
bool Scene::IsChildOfSelected(GameObject* gameObject)
{
    if (gameObject->parent == nullptr) {
        return false; // No tiene padre.
    }
    if (gameObject->parent == selectedGameObject) {
        return true; // Su padre es el seleccionado.
    }
    return IsChildOfSelected(gameObject->parent); // Sigue verificando en la jerarquía.
}

GameObject* Scene::GetRootParent(GameObject* gameObject) {
    if (gameObject->parent == nullptr) {
        return gameObject; // Este objeto es el raíz.
    }
    return GetRootParent(gameObject->parent); // Recurre hacia el padre.
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



bool Scene::intersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax) {
    glm::vec3 tMin = (aabbMin - rayOrigin) / rayDir;
    glm::vec3 tMax = (aabbMax - rayOrigin) / rayDir;

    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar > 0;
}

std::vector<GameObject*> Scene::GetGameObjects()
{

    return gameObjects;
}


void Scene::MoveGameObjectUp(GameObject* gameObject) {
    // Buscar el índice del GameObject seleccionado
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);

    // Si no se encuentra el GameObject en la lista, salir
    if (it == gameObjects.end()) return;

    // Asegurarse de que no es el primer elemento de la lista
    if (it != gameObjects.begin()) {
        // Intercambiar el GameObject con el anterior
        std::iter_swap(it, it - 1);
    }
}

void Scene::MoveGameObjectDown(GameObject* gameObject) {
    // Buscar el índice del GameObject seleccionado
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);

    // Si no se encuentra el GameObject en la lista, salir
    if (it == gameObjects.end()) return;

    // Asegurarse de que no es el último elemento de la lista
    if (it != gameObjects.end() - 1) {
        // Intercambiar el GameObject con el siguiente
        std::iter_swap(it, it + 1);
    }
}

void Scene::DrawGameObjectsRecursively(std::vector<GameObject*> gameObjects) {
    for (GameObject* gameObject : gameObjects) {
        if (gameObject && gameObject->IsEnabled()) {
            gameObject->Draw();  // Dibuja el GameObject actual

            // Si el GameObject tiene hijos, dibuja los hijos recursivamente
            if (!gameObject->GetChildren().empty()) {
                DrawGameObjectsRecursively(gameObject->GetChildren());
            }
        }
    }
}

void Scene::UpdateGameObjectsRecursively(std::vector<GameObject*> gameObjects, float dt)
{
    for (GameObject* gameObject : gameObjects) {
        if (gameObject && gameObject->IsEnabled()) {
            gameObject->Update(dt);  // Dibuja el GameObject actual
            // Si el GameObject tiene hijos, dibuja los hijos recursivamente
            if (!gameObject->GetChildren().empty()) {
                DrawGameObjectsRecursively(gameObject->GetChildren());
            }
        }
    }
}








