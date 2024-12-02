#pragma once
#include <vector>
#include "GameObject.h"
#include "Ray.h"
#ifndef SCENE_H
#define SCENE_H

class App;  // Declaración anticipada para que el compilador conozca la clase App.
class GameObject;
class Scene {
public:
    Scene();


    void Update(double dt);
    void Draw();

    void SaveScene();

    void AddGameObject(GameObject* gameObject);
    void RemoveRootGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);

    void SelectGameObject(GameObject* gameObject);
    /*void SelectGameObject(uint index);*/

    GameObject* GetSelectedGameObject();

    std::vector<GameObject*> GetGameObjectsList();

    void SetCopiedGameObject(GameObject* gameObject);
    GameObject* GetCopiedGameObject();

    void DrawGrid(float size, int divisions);

    void DrawInfiniteGrid(float gridSpacing, int halfGridLines);

    void UpdateMousePicking(int mouseX, int mouseY, int windowWidth, int windowHeight);

    void DrawRay(const Ray& ray, float length);

    void MoveGameObjectUp(GameObject* gameObject);

    void MoveGameObjectDown(GameObject* gameObject);

    void DrawGameObjectsRecursively(std::vector<GameObject*> gameObjects);
    void UpdateGameObjectsRecursively(std::vector<GameObject*> gameObjects, float dt);

    bool RayIntersectsAABB(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax);

    bool intersectsAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax);

    std::vector<GameObject*> GetGameObjects();

    bool isScenePicked = false;

    Ray ray = Ray(glm::vec3(0,0,0), glm::vec3(0, 0, 0));

private:
    std::vector<GameObject*> gameObjects; // Lista de GameObjects en la jerarquía
    GameObject* selectedGameObject; // Lista de GameObjects en la jerarquía

    GameObject* copiedGameObject;
    /*GameObject* GetGameObjectAtRayIntersection(const Ray& ray);*/
};

#endif // SCENE_H
