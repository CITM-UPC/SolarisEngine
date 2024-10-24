#pragma once
#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <IL/il.h>

struct Mesh {
    std::vector<GLfloat> vertices;     // Vértices en 3D
    std::vector<GLfloat> texCoords;    // Coordenadas de textura
    std::vector<GLuint> indices;        // Índices de los vértices

    // Constructor por defecto
    Mesh() = default;

    // Método para agregar vértices
    void AddVertex(GLfloat x, GLfloat y, GLfloat z) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    // Método para agregar coordenadas de textura
    void AddTexCoord(GLfloat u, GLfloat v) {
        texCoords.push_back(u);
        texCoords.push_back(v);
    }

    // Método para agregar índices
    void AddIndex(GLuint index) {
        indices.push_back(index);
    }
};

class Component_Mesh : public Component {
public:
    Component_Mesh(std::shared_ptr<GameObject> containerGO);
    virtual ~Component_Mesh() override;

    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;

    void LoadMesh(aiMesh* ai_mesh);

    // Métodos específicos para el mesh
    //void LoadMesh(const std::string& path);
    void LoadMesh(const aiScene* ai_scene);

    void SetTexture(ILuint textureID);

    GLuint vao;
    std::vector<Mesh> meshes;
    ILuint textureID;

};

#endif // !__COMPONENT_MESH_H__
