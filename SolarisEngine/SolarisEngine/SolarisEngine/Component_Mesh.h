#pragma once
#include "Component.h"
#include "Component_Material.h"
#include "GameObject.h"
#include <vector>
#include <memory>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include "ResourceMesh.h"

//struct Mesh {
//    std::vector<float> vertices;
//    std::vector<float> texCoords;
//    std::vector<unsigned int> indices;
//    std::vector<float> normals; // Normales de los v�rtices
//    std::vector<float> faceNormals;    // Normales por cara
//    Material material;
//};

class Component_Mesh : public Component {
public:
    Component_Mesh(GameObject* containerGO);
    ~Component_Mesh();

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;
    void DrawInspectorComponent() override;

    void LoadMesh(const aiMesh* ai_mesh);
    void LoadMesh(const aiScene* ai_scene);
    void LoadMesh(std::string meshHash);

    void CalculateFaceNormals(ResourceMesh& mesh);

    void SetMaterial(Component_Material* material);

    Component* Clone() const override;

    std::pair<glm::vec3, glm::vec3> GetBoundingBoxInWorldSpace() const;

    // M�todos de creaci�n est�ticos para cada primitivo
    //void GenerateCubeMesh();
    //void GenerateSphereMesh();
    //void GeneratePlaneMesh();
    //void GenerateTriangleMesh();
    //void GenerateCapsuleMesh();
    //void GenerateCylinderMesh();

    glm::vec3 CalculateMeshSize();

    

    std::vector<ResourceMesh*> meshes;
    std::vector<std::string> hashes;

private:
 

    
    int totalVertex = 0;
    GLuint vao;
    Component_Material* material;
    bool showVertexNormals = false;
    bool showFaceNormals = false;
    bool showBoundingBox = false;
    // M�todos auxiliares para definir geometr�as de los distintos tipos de mallas
 
    void SaveMeshes();

};
