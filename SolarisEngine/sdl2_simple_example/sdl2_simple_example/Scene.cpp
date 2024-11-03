#include "Scene.h"

Scene::Scene()
{
}

void Scene::Update(double dt)
{
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
