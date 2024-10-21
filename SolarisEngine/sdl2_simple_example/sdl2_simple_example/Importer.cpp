// Importer.cpp
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
        std::cerr << "Error al cargar el archivo FBX: " << importer.GetErrorString() << std::endl;
        return;
    }

    std::cout << "Archivo FBX cargado exitosamente: " << filepath << std::endl;

    // Procesar materiales
    LoadMaterials(scene);

    // Procesar mallas
    ProcessMeshes(scene);
}

void Importer::LoadMaterials(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* ai_material = scene->mMaterials[i];
        aiString name;
        ai_material->Get(AI_MATKEY_NAME, name);

        materials.push_back(name.C_Str());

        // Cargar textura difusa si existe
        if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString texturePath;
            if (ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
                std::string fullPath = MATERIALS_DIR + std::string("/") + texturePath.C_Str();

                // Cargar la textura con DevIL
                ILuint textureId;
                ilGenImages(1, &textureId);
                ilBindImage(textureId);

                if (ilLoadImage((const wchar_t*)fullPath.c_str())) {
                    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
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

        // Aquí puedes agregar más lógica para procesar los vértices e índices si es necesario
        std::cout << "Malla procesada: " << meshName << " con " << ai_mesh->mNumVertices << " vértices." << std::endl;
    }
}
