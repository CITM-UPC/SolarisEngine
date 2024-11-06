#include "Importer.h"
#include "GameObject.h"
#include "Component_Mesh.h" // Asegúrate de que tienes esta clase definida
#include <memory> // Para std::shared_ptr
#include <filesystem>
#include "App.h"
#include "sstream"

namespace fs = std::filesystem;

Importer::Importer() {
    std::cout << "Inicializando directorios..." << std::endl;
    app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog("Inicializando directorios");

    // Crear directorios si no existen
    for (const auto& dir : { ASSETS_DIR, LIBRARY_DIR, MESHES_DIR, MATERIALS_DIR, MODELS_DIR, FONTS_DIR }) {
        if (!(fs::exists(dir) && fs::is_directory(dir))) {
            fs::create_directory(dir);
            std::cout << "Creado directorio: " << dir << std::endl;
            std::stringstream ss;
            ss << "Creado directorio: " << dir << std::endl;
            app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
        }
    }

    
}

GameObject* Importer::Importar(const std::string& modelPath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    GameObject* newGameObject = GameObject::Create(scene->GetShortFilename(modelPath.c_str()));
    newGameObject->AddComponent<Component_Mesh>();
    Component_Mesh* meshComponent = newGameObject->GetComponent<Component_Mesh>();
    if (meshComponent) {
        meshComponent->LoadMesh(scene);
    }

    return newGameObject;
}

GameObject* Importer::Importar(const std::string& modelPath, const std::string& texturePath) {
    GameObject* newGameObject = Importar(modelPath);
    newGameObject->AddComponent<Component_Material>();
    newGameObject->GetComponent<Component_Material>()->SetTexture(texturePath);

    return newGameObject;
}

unsigned int Importer::LoadTexture(const std::string& texturePath) {
    // Implementación para cargar la textura
    ILuint textureID;
    ilGenImages(1, &textureID);
    ilBindImage(textureID);

    if (!ilLoadImage((const wchar_t*)texturePath.c_str())) {
        std::cerr << "Error al cargar la textura: " << ilGetError() << std::endl;
        return 0; // Retorna 0 si hay un error
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    GLuint glTextureID;
    glGenTextures(1, &glTextureID);
    glBindTexture(GL_TEXTURE_2D, glTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ilDeleteImages(1, &textureID);
    return glTextureID;
}

void Importer::LoadMaterials(const aiScene* scene) {
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* ai_material = scene->mMaterials[i];
        aiString name;
        ai_material->Get(AI_MATKEY_NAME, name);
        std::cout << "Name: " << name.C_Str() << std::endl;
        std::stringstream ss;
        ss << "Name: " << name.C_Str() << std::endl;
        app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
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
                    std::stringstream ss;
                    ss << "Textura cargada: " << fullPath << std::endl;
                    app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
                }
                else {
                    std::cerr << "Error al cargar la textura: " << fullPath << std::endl;
                }

                ilDeleteImages(1, &textureId); // Limpiar ID de textura
            }
        }
    }
}

void Importer::Draw(GameObject* gameObject) {
    if (gameObject) {
        gameObject->Draw(); // Llama al método Draw del GameObject
    }
    else {
        std::cerr << "El GameObject es nulo, no se puede dibujar." << std::endl;
    }
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
    std::stringstream ss;
    ss << "Dibujando modelo: " << modelName << std::endl;
    app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
}
