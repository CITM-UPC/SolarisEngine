#include "Component_Mesh.h"
#include "App.h"
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
            glColor3f(material->GetDiffuseColor().r, material->GetDiffuseColor().g, material->GetDiffuseColor().b);
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

        if (showVertexNormals) {
            glColor3f(0.0f, 1.0f, 0.0f); // Rojo para las normales
            glBegin(GL_LINES);
            for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
                glm::vec3 v(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
                glm::vec3 n(mesh.normals[i], mesh.normals[i + 1], mesh.normals[i + 2]);
                glm::vec3 end = v + n * 0.1f; // Escala de la línea

                glVertex3fv(glm::value_ptr(v));
                glVertex3fv(glm::value_ptr(end));
            }
            glEnd();
            glColor3f(material->GetDiffuseColor().r, material->GetDiffuseColor().g, material->GetDiffuseColor().b);
        }

        if (showFaceNormals) {
            glColor3f(1.0f, 0.0f, 0.0f); // Rojo para las normales de caras
            glBegin(GL_LINES);
            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                // Centro del triángulo
                glm::vec3 v0(mesh.vertices[mesh.indices[i] * 3], mesh.vertices[mesh.indices[i] * 3 + 1], mesh.vertices[mesh.indices[i] * 3 + 2]);
                glm::vec3 v1(mesh.vertices[mesh.indices[i + 1] * 3], mesh.vertices[mesh.indices[i + 1] * 3 + 1], mesh.vertices[mesh.indices[i + 1] * 3 + 2]);
                glm::vec3 v2(mesh.vertices[mesh.indices[i + 2] * 3], mesh.vertices[mesh.indices[i + 2] * 3 + 1], mesh.vertices[mesh.indices[i + 2] * 3 + 2]);
                glm::vec3 center = (v0 + v1 + v2) / 3.0f;

                // Normal de la cara
                glm::vec3 n(mesh.faceNormals[i], mesh.faceNormals[i + 1], mesh.faceNormals[i + 2]);
                glm::vec3 end = center + n * 0.1f;

                glVertex3fv(glm::value_ptr(center));
                glVertex3fv(glm::value_ptr(end));
            }
            glEnd();
            glColor3f(material->GetDiffuseColor().r, material->GetDiffuseColor().g, material->GetDiffuseColor().b);
        }


    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix(); // Restablecer la matriz después de dibujar
}

void Component_Mesh::DrawInspectorComponent()
{
    ImGui::Text("Mesh Component");

    ImGui::Checkbox("Show Vertex Normals", &showVertexNormals); // Activar o desactivar normales
    ImGui::Checkbox("Show Face Normals", &showFaceNormals); // Activar o desactivar normales
}

void Component_Mesh::LoadMesh(aiMesh* ai_mesh) {
   /* glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    this->vao = vao;*/
}

void Component_Mesh::LoadMesh(const aiScene* ai_scene) {
    for (unsigned int i = 0; i < ai_scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = ai_scene->mMeshes[i];

        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);


            if (aiMesh->HasNormals()) { // Copia las normales si existen
                mesh.normals.push_back(aiMesh->mNormals[j].x);
                mesh.normals.push_back(aiMesh->mNormals[j].y);
                mesh.normals.push_back(aiMesh->mNormals[j].z);
            }
            else {
                // Calcula las normales si no están disponibles en el modelo
                // Código para calcular las normales por triángulo
            }


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

        CalculateFaceNormals(mesh);

        meshes.push_back(mesh);
    }

}

void Component_Mesh::CalculateFaceNormals(Mesh& mesh) {
    mesh.faceNormals.clear();
    for (size_t i = 0; i < mesh.indices.size(); i += 3) {
        // Obtén los tres índices del triángulo
        glm::vec3 v0(mesh.vertices[mesh.indices[i] * 3], mesh.vertices[mesh.indices[i] * 3 + 1], mesh.vertices[mesh.indices[i] * 3 + 2]);
        glm::vec3 v1(mesh.vertices[mesh.indices[i + 1] * 3], mesh.vertices[mesh.indices[i + 1] * 3 + 1], mesh.vertices[mesh.indices[i + 1] * 3 + 2]);
        glm::vec3 v2(mesh.vertices[mesh.indices[i + 2] * 3], mesh.vertices[mesh.indices[i + 2] * 3 + 1], mesh.vertices[mesh.indices[i + 2] * 3 + 2]);

        // Calcula el vector normal usando el producto cruzado
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        // Almacena la normal para cada triángulo
        mesh.faceNormals.push_back(normal.x);
        mesh.faceNormals.push_back(normal.y);
        mesh.faceNormals.push_back(normal.z);
    }
}

void Component_Mesh::GenerateCubeMesh() {
    Mesh cubeMesh;
    cubeMesh.vertices = {
        // Frente
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        // Atrás
        -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
    };
    cubeMesh.indices = {
        0, 1, 2, 2, 3, 0, // Frente
        4, 5, 6, 6, 7, 4, // Atrás
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
    // Implementación simplificada de la geometría de una cápsula
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
