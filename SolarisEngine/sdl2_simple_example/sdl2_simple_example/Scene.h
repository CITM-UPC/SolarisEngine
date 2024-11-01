#pragma once
#include <vector>
#include "Component.h"
#include "GameObject.h"
#ifndef SCENE_H
#define SCENE_H

class App;  // Declaración anticipada para que el compilador conozca la clase App.
class GameObject;
class Scene {
public:
    Scene();


    void Update(double dt);

    void SaveScene();

    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);

    void SelectGameObject(GameObject* gameObject);
    void SelectGameObject(uint index);

    GameObject* GetSelectedGameObject();

    std::vector<GameObject*> GetGameObjectsList();


private:
    std::vector<GameObject*> gameObjects; // Lista de GameObjects en la jerarquía
    GameObject* selectedGameObject; // Lista de GameObjects en la jerarquía
};

#endif // SCENE_H
