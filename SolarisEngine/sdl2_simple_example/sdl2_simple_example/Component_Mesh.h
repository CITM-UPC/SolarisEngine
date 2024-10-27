#pragma once
#include "Component.h"
#include "Component_Material.h"
#include "GameObject.h" // Asegúrate de incluir esto
#include <vector>
#include <memory>
#include <assimp/scene.h>  // Incluye Assimp para la importación de mallas
#include <GL/glew.h>
#include <gl/GL.h>

class Component_Mesh : public Component {
public:
    Component_Mesh(GameObject* containerGO); // Cambiado a puntero crudo
    ~Component_Mesh();

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    void LoadMesh(aiMesh* ai_mesh);
    void LoadMesh(const aiScene* ai_scene);

    void SetMaterial(Component_Material* material);  // Cambiado a puntero crudo

private:
    struct Mesh {
        std::vector<float> vertices;
        std::vector<float> texCoords;
        std::vector<unsigned int> indices;
    };

    std::vector<Mesh> meshes;
    GLuint vao;
    Component_Material* material; // Cambiado a puntero crudo
};
