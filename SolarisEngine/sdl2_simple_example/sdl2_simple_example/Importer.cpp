#include "Importer.h"
namespace fs = std::filesystem;

Importer::Importer() {
    std::cout << "Inicializando directorios..." << std::endl;

    // Crear directorios si no existen
    for (const auto& dir : { ASSETS_DIR, LIBRARY_DIR, MESHES_DIR, MATERIALS_DIR, MODELS_DIR }) {
        if (!(fs::exists(dir) && fs::is_directory(dir))) {
            fs::create_directory(dir);
            std::cout << "Creado directorio: " << dir << std::endl;
        }
    }

    ilInit(); // Inicializa DevIL
}

void Importer::Importar(const std::string& filepath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene) {
        std::cerr << "Error al cargar el archivo: " << importer.GetErrorString() << std::endl;
        return;
    }

    std::cout << "Archivo cargado exitosamente: " << filepath << std::endl;

    LoadMaterials(scene);
    ProcessMeshes(scene);
}

void Importer::LoadMaterials(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* ai_material = scene->mMaterials[i];
        aiString name;
        ai_material->Get(AI_MATKEY_NAME, name);
        std::cout << "Name: " << name.C_Str() << std::endl;
        materials.push_back(name.C_Str());

        // Cargar textura difusa si existe
        if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString texturePath;
            if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
                std::string fullPath = MATERIALS_DIR + std::string("/") + texturePath.C_Str();
                GLuint textureId;
                ilGenImages(1, &textureId);
                ilBindImage(textureId);

                if (ilLoadImage((const wchar_t*)fullPath.c_str())) {
                    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
                    glGenTextures(1, &textureId);
                    glBindTexture(GL_TEXTURE_2D, textureId);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
                    glGenerateMipmap(GL_TEXTURE_2D);

                    textureIds[name.C_Str()] = textureId; // Guardar ID de textura
                    std::cout << "Textura cargada: " << fullPath << std::endl;
                   
                }
                else {
                    std::cerr << "Error al cargar la textura: " << fullPath << std::endl;
                }

                ilDeleteImages(1, &textureId); // Limpiar ID de textura
            }
        }
    }
}

void Importer::ProcessMeshes(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* ai_mesh = scene->mMeshes[i];
        std::string meshName = ai_mesh->mName.C_Str();
        meshes.push_back(meshName);

        // Aquí puedes almacenar los VBO y VAO
        // Ejemplo simple:
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, ai_mesh->mNumVertices * sizeof(aiVector3D), ai_mesh->mVertices, GL_STATIC_DRAW);

        // Configura tus atributos aquí (posiciones, normales, etc.)
        // Ejemplo:
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Puedes almacenar el VAO y otros datos necesarios en un mapa
        // vaos[meshName] = vao; // Asegúrate de declarar esto como un mapa en tu clase

        std::cout << "Malla procesada: " << meshName << " con " << ai_mesh->mNumVertices << " vértices." << std::endl;
    }
}

GLuint Importer::GetTextureIdForModel(const std::string& modelName) {
    auto it = textureIds.find(modelName);
    return (it != textureIds.end()) ? it->second : 0; // Devuelve 0 si no se encuentra
}

void Importer::Draw(const std::string& modelName) {
    auto it = vaos.find(modelName);
    if (it == vaos.end()) {
        std::cerr << "Modelo no encontrado: " << modelName << std::endl;
        return;
    }

    GLuint vao = it->second; // Obtener el VAO
    glBindVertexArray(vao);

    // Aquí asumiendo que tienes un método para obtener el número de índices
    // glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    std::cout << "Dibujando modelo: " << modelName << std::endl;
}
