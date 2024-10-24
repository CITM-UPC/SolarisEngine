#include "Component_Mesh.h"
#include <iostream>

//Component_Mesh::Component_Mesh(std::shared_ptr<GameObject> containerGO)
//    : Component(containerGO, ComponentType::Mesh) {}

Component_Mesh::Component_Mesh(std::shared_ptr<GameObject> containerGO) : Component(containerGO, ComponentType::Mesh)
{
}

Component_Mesh::~Component_Mesh() {}

void Component_Mesh::Enable() {
    enabled = true;
    // Lógica para habilitar el componente
}

void Component_Mesh::Disable() {
    enabled = false;
    // Lógica para deshabilitar el componente
}

void Component_Mesh::Update(double dt) {
    // Lógica de actualización
}

void Component_Mesh::DrawComponent() {
    // Lógica para dibujar el mesh
    for (const auto& mesh : meshes) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
    }

}

void Component_Mesh::LoadMesh(aiMesh* ai_mesh) {
    // Configura tu VAO y VBO aquí
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Guarda el VAO y otros datos necesarios para el dibujo
    this->vao = vao; // Asegúrate de tener un campo para almacenar esto
}
