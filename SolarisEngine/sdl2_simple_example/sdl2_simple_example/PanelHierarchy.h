#pragma once

#include <vector>
#include <string>
#include "GameObject.h" // Asegúrate de incluir tu clase GameObject

class PanelHierarchy {
public:
    PanelHierarchy();
    ~PanelHierarchy();

    void Render(); // Método para renderizar el panel de jerarquía
    void AddGameObject(GameObject* gameObject); // Método para añadir un GameObject a la jerarquía

private:
    std::vector<GameObject*> gameObjects; // Lista de GameObjects en la jerarquía
    void DrawGameObject(GameObject* gameObject); // Método para dibujar un GameObject
};

