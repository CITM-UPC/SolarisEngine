#pragma once

#include <vector>
#include <string>
#include "GameObject.h" // Aseg�rate de incluir tu clase GameObject

class PanelHierarchy {
public:
    PanelHierarchy();
    ~PanelHierarchy();

    void Render(); // M�todo para renderizar el panel de jerarqu�a
    void AddGameObject(GameObject* gameObject); // M�todo para a�adir un GameObject a la jerarqu�a

private:
    std::vector<GameObject*> gameObjects; // Lista de GameObjects en la jerarqu�a
    void DrawGameObject(GameObject* gameObject); // M�todo para dibujar un GameObject
};

