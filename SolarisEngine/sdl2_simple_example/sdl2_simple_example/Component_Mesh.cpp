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
    // L骻ica de actualizaci髇 si es necesario
}

void Component_Mesh::DrawComponent() {
    if (!enabled) return;

    // Obtener el GameObject contenedor
    //auto containerGOLock = containerGO.lock();
    //if (!containerGOLock) {
    //    std::cerr << "Error: El GameObject asociado ya no est?disponible." << std::endl;
    //    return; // El objeto ha sido destruido
    //}

    // Obtener la matriz de transformaci髇 desde Component_Transform
    auto transform = containerGO->GetComponent<Component_Transform>();
    if (!transform) {
        std::cerr << "Error: No se pudo obtener el componente de transformaci髇." << std::endl;
        return;
    }
    glm::mat4 modelMatrix = transform->GetModelMatrix();

    // Aplicar la matriz de transformaci髇 en el modelo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(modelMatrix));

    // Configurar el material y textura (similar a lo que ya tienes)
    if (material) {
        GLuint textureID = material->GetTextureID();
        if (textureID == 0) {
            std::cerr << "Advertencia: Textura no v醠ida en material. Usando color rosa de error." << std::endl;
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
                glm::vec3 end = v + n * 0.1f; // Escala de la l韓ea

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
                // Centro del tri醤gulo
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
    glPopMatrix(); // Restablecer la matriz despu閟 de dibujar
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
                // Calcula las normales si no est醤 disponibles en el modelo
                // C骴igo para calcular las normales por tri醤gulo
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
        // Obt閚 los tres 韓dices del tri醤gulo
        glm::vec3 v0(mesh.vertices[mesh.indices[i] * 3], mesh.vertices[mesh.indices[i] * 3 + 1], mesh.vertices[mesh.indices[i] * 3 + 2]);
        glm::vec3 v1(mesh.vertices[mesh.indices[i + 1] * 3], mesh.vertices[mesh.indices[i + 1] * 3 + 1], mesh.vertices[mesh.indices[i + 1] * 3 + 2]);
        glm::vec3 v2(mesh.vertices[mesh.indices[i + 2] * 3], mesh.vertices[mesh.indices[i + 2] * 3 + 1], mesh.vertices[mesh.indices[i + 2] * 3 + 2]);

        // Calcula el vector normal usando el producto cruzado
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        // Almacena la normal para cada tri醤gulo
        mesh.faceNormals.push_back(normal.x);
        mesh.faceNormals.push_back(normal.y);
        mesh.faceNormals.push_back(normal.z);
    }
}

void Component_Mesh::GenerateCubeMesh() {
    Mesh cubeMesh;
    cubeMesh.vertices = {
        // 前面 (Frente)
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

        // 后面 (Atrás)
        -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

        // 左面 (Izquierda)
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,

        // 右面 (Derecha)
         0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,

         // 上面 (Arriba)
         -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,

         // 下面 (Abajo)
         -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
    };

    cubeMesh.indices = {
        // 前面 (Frente)
        0, 1, 2, 2, 3, 0,

        // 后面 (Atrás)
        4, 5, 6, 6, 7, 4,

        // 左面 (Izquierda)
        8, 9, 10, 10, 11, 8,

        // 右面 (Derecha)
        12, 13, 14, 14, 15, 12,

        // 上面 (Arriba)
        16, 17, 18, 18, 19, 16,

        // 下面 (Abajo)
        20, 21, 22, 22, 23, 20,
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
    Mesh tetrahedronMesh;

    // 更对称的四面体顶点坐标
    const float sqrt2div3 = sqrt(2.0f / 3.0f);
    const float sqrt6div3 = sqrt(6.0f) / 3.0f;
    const float sqrt3div3 = sqrt(3.0f) / 3.0f;

    tetrahedronMesh.vertices = {
        1.0f, 1.0f, 1.0f,                     // 顶点 A
        -1.0f, -1.0f, 1.0f,                   // 顶点 B
        -1.0f, 1.0f, -1.0f,                   // 顶点 C
        1.0f, -1.0f, -1.0f                    // 顶点 D
    };

    // 每个面由三个顶点组成
    tetrahedronMesh.indices = {
        0, 1, 2,  // 面 ABC
        0, 3, 1,  // 面 ABD
        0, 2, 3,  // 面 ACD
        1, 3, 2   // 面 BCD
    };

    meshes.push_back(tetrahedronMesh);
}

void Component_Mesh::GenerateCapsuleMesh() {
    Mesh capsuleMesh;

    // Fixed parameters for the capsule size
    const float height = 2.0f; // Total height of the capsule
    const float radius = 0.5f;  // Radius of the capsule
    const int numSegments = 16;  // Number of segments for smoothness
    const int numStacks = 8;      // Number of stacks for hemispheres

    // Create vertices for the cylindrical part
    for (int i = 0; i < numSegments; ++i) {
        float angle = i * (2.0f * M_PI / numSegments);
        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        // Bottom circle vertices
        capsuleMesh.vertices.push_back(x);
        capsuleMesh.vertices.push_back(-height / 2); // Bottom
        capsuleMesh.vertices.push_back(z);

        // Top circle vertices
        capsuleMesh.vertices.push_back(x);
        capsuleMesh.vertices.push_back(height / 2); // Top
        capsuleMesh.vertices.push_back(z);
    }

    // Create vertices for the top hemisphere
    for (int i = 0; i <= numSegments; ++i) {
        for (int j = 0; j <= numStacks; ++j) {
            float theta = j * (M_PI / numStacks); // Polar angle
            float phi = i * (2.0f * M_PI / numSegments); // Azimuthal angle

            float x = cos(phi) * sin(theta) * radius;
            float y = cos(theta) * radius + (height / 2); // Offset by height
            float z = sin(phi) * sin(theta) * radius;

            capsuleMesh.vertices.push_back(x);
            capsuleMesh.vertices.push_back(y);
            capsuleMesh.vertices.push_back(z);
        }
    }

    // Create vertices for the bottom hemisphere
    for (int i = 0; i <= numSegments; ++i) {
        for (int j = 0; j <= numStacks; ++j) {
            float theta = j * (M_PI / numStacks); // Polar angle
            float phi = i * (2.0f * M_PI / numSegments); // Azimuthal angle

            float x = cos(phi) * sin(theta) * radius;
            float y = -cos(theta) * radius - (height / 2); // Offset by height
            float z = sin(phi) * sin(theta) * radius;

            capsuleMesh.vertices.push_back(x);
            capsuleMesh.vertices.push_back(y);
            capsuleMesh.vertices.push_back(z);
        }
    }

    // Create indices for the cylindrical part
    for (int i = 0; i < numSegments; ++i) {
        int bottom1 = i * 2;
        int bottom2 = (i + 1) % numSegments * 2;
        int top1 = bottom1 + 1;
        int top2 = bottom2 + 1;

        // First triangle
        capsuleMesh.indices.push_back(bottom1);
        capsuleMesh.indices.push_back(bottom2);
        capsuleMesh.indices.push_back(top1);

        // Second triangle
        capsuleMesh.indices.push_back(top1);
        capsuleMesh.indices.push_back(bottom2);
        capsuleMesh.indices.push_back(top2);
    }

    // Create indices for the top hemisphere
    int baseIndex = numSegments * 2; // Start index for hemispheres
    for (int i = 0; i < numSegments; ++i) {
        for (int j = 0; j < numStacks; ++j) {
            int current = baseIndex + i * (numStacks + 1) + j;
            int next = baseIndex + (i + 1) % numSegments * (numStacks + 1) + j;
            int top = baseIndex + i * (numStacks + 1) + (j + 1);

            // First triangle
            capsuleMesh.indices.push_back(current);
            capsuleMesh.indices.push_back(next);
            capsuleMesh.indices.push_back(top);
        }
    }

    // Create indices for the bottom hemisphere
    baseIndex += (numSegments * (numStacks + 1));
    for (int i = 0; i < numSegments; ++i) {
        for (int j = 0; j < numStacks; ++j) {
            int current = baseIndex + i * (numStacks + 1) + j;
            int next = baseIndex + (i + 1) % numSegments * (numStacks + 1) + j;
            int bottom = baseIndex + i * (numStacks + 1) + (j + 1);

            // First triangle
            capsuleMesh.indices.push_back(current);
            capsuleMesh.indices.push_back(bottom);
            capsuleMesh.indices.push_back(next);
        }
    }

    // Store the mesh
    meshes.push_back(capsuleMesh);
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

glm::vec3 Component_Mesh::CalculateMeshSize() {
    if (meshes.empty()) {
        return glm::vec3(0.0f); // Retornar un tama駉 cero si no hay mallas
    }

    glm::vec3 minBounds(FLT_MAX, FLT_MAX, FLT_MAX); // Inicializar con valores m醲imos
    glm::vec3 maxBounds(-FLT_MAX, -FLT_MAX, -FLT_MAX); // Inicializar con valores m韓imos

    for (const auto& mesh : meshes) {
        // Asumiendo que 'mesh.vertices' es un vector plano que contiene las coordenadas de los v閞tices
        for (size_t i = 0; i < mesh.vertices.size(); i += 3) {
            glm::vec3 vertex(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);

            // Encontrar los l韒ites m韓imos y m醲imos en cada eje
            minBounds = glm::min(minBounds, vertex);
            maxBounds = glm::max(maxBounds, vertex);
        }
    }

    // Calcular el tama駉 de la malla en cada dimensi髇
    glm::vec3 meshSize = maxBounds - minBounds;
    std::cout << "Mesh size: " << meshSize.x << ", " << meshSize.y << ", " << meshSize.z << std::endl;

    return meshSize; // Retornar el tama駉 de la malla
}




