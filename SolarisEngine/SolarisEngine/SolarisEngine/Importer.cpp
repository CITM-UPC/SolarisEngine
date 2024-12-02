#include "Importer.h"
#include "GameObject.h"
#include "Component_Mesh.h" // Aseg�rate de que tienes esta clase definida
#include <memory> // Para std::shared_ptr
#include <filesystem>
#include "App.h"
#include "sstream"
#include "Debug.h"

namespace fs = std::filesystem;

Importer::Importer() {
    std::cout << "Inicializando directorios..." << std::endl;
    Debug::Log("Inicializando directorios");

    // Crear directorios si no existen
    for (const auto& dir : { ASSETS_DIR, LIBRARY_DIR, MESHES_DIR, MATERIALS_DIR, MODELS_DIR, FONTS_DIR, TEXTURES_DIR }) {
        if (!(fs::exists(dir) && fs::is_directory(dir))) {
            fs::create_directory(dir);
            std::cout << "Creado directorio: " << dir << std::endl;
            std::stringstream ss;
            ss << "Creado directorio: " << dir << std::endl;
            app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
        }
    }

    
}


GameObject* Importer::ImportarNuevo(const std::string& modelPath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    // Crear el GameObject ra�z
    GameObject* newGameObject = GameObject::Create(scene->GetShortFilename(modelPath.c_str()));

    // Aseg�rate de que el componente Transform est� presente
    if (!newGameObject->GetComponent<Component_Transform>()) {
        newGameObject->AddComponent<Component_Transform>();
    }

    // Aseg�rate de que el GameObject ra�z tenga un componente de malla
    for (unsigned int i = 0; i < scene->mRootNode->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];
        Component_Mesh* meshComponent = newGameObject->GetComponent<Component_Mesh>();
        if (!meshComponent) {
            meshComponent = newGameObject->AddComponent<Component_Mesh>();
        }
        meshComponent->LoadMesh(aiMesh);
    }

    // Importar los hijos de la ra�z
    ImportarChilds(scene->mRootNode, scene, newGameObject);

    return newGameObject;
}

void Importer::ImportarChilds(aiNode* node, const aiScene* scene, GameObject* parent) {
    // Crear un GameObject para este nodo
    GameObject* newGameObject = GameObject::Create(node->mName.C_Str());
    //parent->AddChild(newGameObject);
    newGameObject->SetParent(parent);

    // Asegurarse de que el componente Transform est� presente en el GameObject
    if (!newGameObject->GetComponent<Component_Transform>()) {
        newGameObject->AddComponent<Component_Transform>();
    }

    // Aplicar la transformaci�n del nodo al GameObject
    aiMatrix4x4 aiTransform = node->mTransformation;
    glm::mat4 transform = AssimpToGLM(aiTransform);
    glm::vec3 scale = glm::vec3(transform[0][0], transform[1][1], transform[2][2]);
    Debug::Log("Escala del nodo ", node->mName.C_Str(), ": ", scale.x, " ", scale.y, " ", scale.z);
    //newGameObject->GetComponent<Component_Transform>()->ApplyMatrix(transform);

    // Cargar las mallas asociadas a este nodo
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        Component_Mesh* meshComponent = newGameObject->GetComponent<Component_Mesh>();
        if (meshComponent) {
            meshComponent->LoadMesh(aiMesh);
        }
        else {
            meshComponent = newGameObject->AddComponent<Component_Mesh>();
            meshComponent->LoadMesh(aiMesh);
        }
    }

    // Recursi�n para procesar los hijos de este nodo
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ImportarChilds(node->mChildren[i], scene, newGameObject); // Llamada recursiva para los hijos
    }
}

glm::mat4 Importer::AssimpToGLM(const aiMatrix4x4& mat) {
    glm::mat4 glmMat(
        mat.a1, mat.a2, mat.a3, mat.a4,
        mat.b1, mat.b2, mat.b3, mat.b4,
        mat.c1, mat.c2, mat.c3, mat.c4,
        mat.d1, mat.d2, mat.d3, mat.d4
    );
    return glmMat;
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
        Debug::Log("Cargando mesh:", scene->GetShortFilename(modelPath.c_str()));
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
    // Implementaci�n para cargar la textura
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
        gameObject->Draw(); // Llama al m�todo Draw del GameObject
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

    // Aqu� asumiendo que tienes un m�todo para obtener el n�mero de �ndices
    // glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

    std::cout << "Dibujando modelo: " << modelName << std::endl;
    std::stringstream ss;
    ss << "Dibujando modelo: " << modelName << std::endl;
    app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
}
