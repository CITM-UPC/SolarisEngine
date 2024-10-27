#include "Component_Mesh.h"
#include <iostream>
#include "Component_Transform.h"
#include <SDL2/SDL_stdinc.h>

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
    // L�gica de actualizaci�n si es necesario
}

void Component_Mesh::DrawComponent() {
    if (!enabled) return;

    // Obtener el GameObject contenedor
    //auto containerGOLock = containerGO.lock();
    //if (!containerGOLock) {
    //    std::cerr << "Error: El GameObject asociado ya no est� disponible." << std::endl;
    //    return; // El objeto ha sido destruido
    //}

    // Obtener la matriz de transformaci�n desde Component_Transform
    auto transform = containerGO->GetComponent<Component_Transform>();
    if (!transform) {
        std::cerr << "Error: No se pudo obtener el componente de transformaci�n." << std::endl;
        return;
    }
    glm::mat4 modelMatrix = transform->GetModelMatrix();

    // Aplicar la matriz de transformaci�n en el modelo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(modelMatrix));

    // Configurar el material y textura (similar a lo que ya tienes)
    if (material) {
        GLuint textureID = material->GetTextureID();
        if (textureID == 0) {
            std::cerr << "Advertencia: Textura no v�lida en material. Usando color rosa de error." << std::endl;
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
    glPopMatrix(); // Restablecer la matriz despu�s de dibujar
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

void Component_Mesh::GenerateCubeMesh() {
    Mesh cubeMesh;
    cubeMesh.vertices = {
        // Frente
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        // Atr�s
        -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
    };
    cubeMesh.indices = {
        0, 1, 2, 2, 3, 0, // Frente
        4, 5, 6, 6, 7, 4, // Atr�s
        // Otros lados...
    };
    meshes.push_back(cubeMesh);
}

void Component_Mesh::GenerateSphereMesh() {
    Mesh sphereMesh;
    const int sectorCount = 36;
    const int stackCount = 18;
    const float radius = 0.5f;
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stackCount;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * M_PI / sectorCount;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            sphereMesh.vertices.push_back(x);
            sphereMesh.vertices.push_back(y);
            sphereMesh.vertices.push_back(z);
        }
    }
    for (int i = 0; i < stackCount; ++i) {
        for (int j = 0; j < sectorCount; ++j) {
            int first = (i * (sectorCount + 1)) + j;
            int second = first + sectorCount + 1;
            sphereMesh.indices.push_back(first);
            sphereMesh.indices.push_back(second);
            sphereMesh.indices.push_back(first + 1);
            sphereMesh.indices.push_back(second);
            sphereMesh.indices.push_back(second + 1);
            sphereMesh.indices.push_back(first + 1);
        }
    }
    meshes.push_back(sphereMesh);
}

void Component_Mesh::GeneratePlaneMesh() {
    Mesh planeMesh;
    planeMesh.vertices = { -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f };
    planeMesh.indices = { 0, 1, 2, 2, 3, 0 };
    meshes.push_back(planeMesh);
}

void Component_Mesh::GenerateTriangleMesh() {
    Mesh triangleMesh;
    triangleMesh.vertices = { 0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f };
    triangleMesh.indices = { 0, 1, 2 };
    meshes.push_back(triangleMesh);
}

void Component_Mesh::GenerateCapsuleMesh() {
    // Implementaci�n simplificada de la geometr�a de una c�psula
    GenerateCylinderMesh();
    GenerateSphereMesh(); // Semiesfera en la parte superior
    GenerateSphereMesh(); // Semiesfera en la parte inferior
}

void Component_Mesh::GenerateCylinderMesh() {
    Mesh cylinderMesh;
    const int sectorCount = 36;
    const float radius = 0.5f;
    const float height = 1.0f;
    for (int i = 0; i < sectorCount; ++i) {
        float angle = i * 2 * M_PI / sectorCount;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        cylinderMesh.vertices.push_back(x);
        cylinderMesh.vertices.push_back(y);
        cylinderMesh.vertices.push_back(-height / 2);
        cylinderMesh.vertices.push_back(x);
        cylinderMesh.vertices.push_back(y);
        cylinderMesh.vertices.push_back(height / 2);
    }
    for (int i = 0; i < sectorCount - 1; ++i) {
        cylinderMesh.indices.push_back(i * 2);
        cylinderMesh.indices.push_back(i * 2 + 1);
        cylinderMesh.indices.push_back(i * 2 + 2);
        cylinderMesh.indices.push_back(i * 2 + 1);
        cylinderMesh.indices.push_back(i * 2 + 3);
        cylinderMesh.indices.push_back(i * 2 + 2);
    }
    meshes.push_back(cylinderMesh);
}
