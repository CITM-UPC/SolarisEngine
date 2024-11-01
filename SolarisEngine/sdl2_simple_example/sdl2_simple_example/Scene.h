#pragma once
#include <vector>
#include "Component.h"
#include "GameObject.h"
#ifndef SCENE_H
#define SCENE_H

class App;  // Declaraci�n anticipada para que el compilador conozca la clase App.
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
    std::vector<GameObject*> gameObjects; // Lista de GameObjects en la jerarqu�a
    GameObject* selectedGameObject; // Lista de GameObjects en la jerarqu�a
};

#endif // SCENE_H
