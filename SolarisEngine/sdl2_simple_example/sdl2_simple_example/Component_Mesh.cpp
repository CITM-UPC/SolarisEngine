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
    // L�gica para habilitar el componente
}

void Component_Mesh::Disable() {
    enabled = false;
    // L�gica para deshabilitar el componente
}

void Component_Mesh::Update(double dt) {
    // L�gica de actualizaci�n
}
void Component_Mesh::DrawComponent() {
    // Si no hay una textura v�lida, cambiamos el color a rosa (1.0, 0.0, 1.0)
    if (textureID == 0) {
        std::cerr << "Error: Textura no v�lida. Usando color rosa de error." << std::endl;
        glBindTexture(GL_TEXTURE_2D, 0); // Desactiva la textura
        glColor3f(1.0f, 0.0f, 1.0f); // Color rosa (RGB: 1.0, 0.0, 1.0)
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f); // Color blanco si hay textura
        glBindTexture(GL_TEXTURE_2D, textureID); // Activa la textura
    }

    // Habilita el estado del array de v�rtices
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, nullptr); // Usaremos nullptr y estableceremos el puntero en el bucle

    for (const auto& mesh : meshes) {
        // Asigna el puntero de v�rtices para la malla actual
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());

        // Habilita el array de coordenadas de textura si lo necesitas
        if (!mesh.texCoords.empty() && textureID != 0) { // Si hay textura y es v�lida
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh.texCoords.data());
        }

        // Dibuja la malla
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());

        // Deshabilita el array de coordenadas de textura si fue habilitado
        if (!mesh.texCoords.empty() && textureID != 0) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }

    // Deshabilita el estado del array de v�rtices
    glDisableClientState(GL_VERTEX_ARRAY);
}





void Component_Mesh::LoadMesh(aiMesh* ai_mesh) {
    // Configura tu VAO y VBO aqu�
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Guarda el VAO y otros datos necesarios para el dibujo
    this->vao = vao; // Aseg�rate de tener un campo para almacenar esto
}

void Component_Mesh::LoadMesh(const aiScene* ai_scene)
{
    for (unsigned int i = 0; i < ai_scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = ai_scene->mMeshes[i];

        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);

            // Cargar las coordenadas de textura si est�n disponibles
            if (aiMesh->mTextureCoords[0]) { // Comprobamos si hay coordenadas de textura
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].x);
                mesh.texCoords.push_back(aiMesh->mTextureCoords[0][j].y);
            }
            else {
                // Si no hay coordenadas de textura, puedes asignar (0,0) o cualquier valor por defecto
                mesh.texCoords.push_back(0.0f);
                mesh.texCoords.push_back(0.0f);
            }

            // Cargar las normales si est�n disponibles
            if (aiMesh->mNormals) {
                // Aqu� puedes almacenar las normales si necesitas usarlas
                // Por ejemplo: normals.push_back(aiMesh->mNormals[j].x); // Puedes a�adir un vector normals en la malla
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

void Component_Mesh::SetTexture(ILuint textureID)
{
    this->textureID = textureID;
}
