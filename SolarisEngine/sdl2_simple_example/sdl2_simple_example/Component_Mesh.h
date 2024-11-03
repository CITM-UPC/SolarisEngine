#pragma once
#include "Component.h"
#include "Component_Material.h"
#include "GameObject.h"
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <gl/GL.h>


struct Mesh {
    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<float> normals; // Normales de los vértices
    std::vector<float> faceNormals;    // Normales por cara
};

class Component_Mesh : public Component {
public:
    Component_Mesh(GameObject* containerGO);
    ~Component_Mesh();

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;
    void DrawInspectorComponent() override;

    void LoadMesh(aiMesh* ai_mesh);
    void LoadMesh(const aiScene* ai_scene);

    void CalculateFaceNormals(Mesh& mesh);

    void SetMaterial(Component_Material* material);

    Component* Clone() const override;

    // Métodos de creación estáticos para cada primitivo
    void GenerateCubeMesh();
    void GenerateSphereMesh();
    void GeneratePlaneMesh();
    void GenerateTriangleMesh();
    void GenerateCapsuleMesh();
    void GenerateCylinderMesh();

    glm::vec3 CalculateMeshSize();

    

    

private:
 

    std::vector<Mesh> meshes;
    GLuint vao;
    Component_Material* material;
    bool showVertexNormals = false;;
    bool showFaceNormals = false;;

    // Métodos auxiliares para definir geometrías de los distintos tipos de mallas
 
};
