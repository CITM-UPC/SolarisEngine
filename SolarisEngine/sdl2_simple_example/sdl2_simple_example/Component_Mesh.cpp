#include "Component_Mesh.h"
#include <iostream>
#include "Component_Transform.h"

Component_Mesh::Component_Mesh(GameObject* containerGO)  // Cambiado a puntero crudo
    : Component(containerGO, ComponentType::Mesh), vao(0), material(nullptr) {
    // Intenta obtener el componente de material del GameObject
    material = containerGO->GetComponent<Component_Material>();
}

Component_Mesh::~Component_Mesh() {}

void Component_Mesh::Enable() {
    enabled = true;
}

void Component_Mesh::Disable() {
    enabled = false;
}

void Component_Mesh::Update(double dt) {
    // Lógica de actualización si es necesario
}

void Component_Mesh::DrawComponent() {
    if (!enabled) return;

    // Obtener el GameObject contenedor
    //auto containerGOLock = containerGO.lock();
    //if (!containerGOLock) {
    //    std::cerr << "Error: El GameObject asociado ya no está disponible." << std::endl;
    //    return; // El objeto ha sido destruido
    //}

    // Obtener la matriz de transformación desde Component_Transform
    auto transform = containerGO->GetComponent<Component_Transform>();
    if (!transform) {
        std::cerr << "Error: No se pudo obtener el componente de transformación." << std::endl;
        return;
    }
    glm::mat4 modelMatrix = transform->GetModelMatrix();

    // Aplicar la matriz de transformación en el modelo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(modelMatrix));

    // Configurar el material y textura (similar a lo que ya tienes)
    if (material) {
        GLuint textureID = material->GetTextureID();
        if (textureID == 0) {
            std::cerr << "Advertencia: Textura no válida en material. Usando color rosa de error." << std::endl;
            glBindTexture(GL_TEXTURE_2D, 0);
            glColor3f(1.0f, 0.0f, 1.0f); // Color rosa de error
        }
        else {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
        glColor3f(1.0f, 0.0f, 1.0f); // Color rosa si no hay material
        material = containerGO->GetComponent<Component_Material>(); // No usar shared_ptr
    }

    // Dibujo de la malla usando OpenGL
    glEnableClientState(GL_VERTEX_ARRAY);

    for (const auto& mesh : meshes) {
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());

        if (!mesh.texCoords.empty() && material && material->GetTextureID() != 0) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.texCoords.data());
        }

        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

        if (!mesh.texCoords.empty() && material && material->GetTextureID() != 0) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix(); // Restablecer la matriz después de dibujar
}

void Component_Mesh::LoadMesh(aiMesh* ai_mesh) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->vao = vao;
}

void Component_Mesh::LoadMesh(const aiScene* ai_scene) {
    for (unsigned int i = 0; i < ai_scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = ai_scene->mMeshes[i];

        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);

            if (aiMesh->mTextureCoords[0]) {
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].x);
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].y);
            }
            else {
                mesh.texCoords.push_back(0.0f);
                mesh.texCoords.push_back(0.0f);
            }
        }

        for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j) {
            const aiFace& face = aiMesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                mesh.indices.push_back(face.mIndices[k]);
            }
        }

        meshes.push_back(mesh);
    }
}
