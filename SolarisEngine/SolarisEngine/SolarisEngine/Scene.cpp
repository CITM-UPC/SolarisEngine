#include "Scene.h"
#include "App.h"

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

void Scene::SelectGameObject(uint index)
{
    if(index >= 0 && index < gameObjects.size())
        selectedGameObject = gameObjects.at(index);
}

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

