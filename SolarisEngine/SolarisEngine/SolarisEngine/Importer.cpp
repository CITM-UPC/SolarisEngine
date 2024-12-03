#include "Importer.h"
#include "GameObject.h"
#include "Component_Mesh.h" // Asegúrate de que tienes esta clase definida
#include "Component_Material.h" // Asegúrate de que tienes esta clase definida
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

    // Crear el GameObject raíz
    GameObject* newGameObject = GameObject::Create(scene->GetShortFilename(modelPath.c_str()));

    // Asegúrate de que el componente Transform esté presente
    if (!newGameObject->GetComponent<Component_Transform>()) {
        newGameObject->AddComponent<Component_Transform>();
    }

    // Importar las mallas y los materiales
    for (unsigned int i = 0; i < scene->mRootNode->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];

        // Crear o obtener el componente de malla
        Component_Mesh* meshComponent = newGameObject->GetComponent<Component_Mesh>();
        if (!meshComponent) {
            meshComponent = newGameObject->AddComponent<Component_Mesh>();
        }
        meshComponent->LoadMesh(aiMesh);

        // Obtener el material asociado a esta malla
        aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

        // Crear o obtener el componente de material
        Component_Material* materialComponent = newGameObject->GetComponent<Component_Material>();
        if (!materialComponent) {
            materialComponent = newGameObject->AddComponent<Component_Material>();
        }

        // Cargar texturas y propiedades del material
        LoadTexturesFromMaterial(material, materialComponent, scene, modelPath);
        LoadMaterialProperties(material, materialComponent);
    }

    // Importar los hijos del nodo raíz
    ImportarChilds(scene->mRootNode, scene, newGameObject, modelPath);

    return newGameObject;
}

void Importer::ImportarChilds(aiNode* node, const aiScene* scene, GameObject* parent, const std::string& modelPath) {
    // Crear un GameObject para este nodo
    GameObject* newGameObject = GameObject::Create(node->mName.C_Str());
    newGameObject->SetParent(parent);

    // Asegurarse de que el componente Transform esté presente en el GameObject
    if (!newGameObject->GetComponent<Component_Transform>()) {
        newGameObject->AddComponent<Component_Transform>();
    }

    // Cargar las mallas y los materiales del nodo
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        const aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

        // Crear o obtener el componente de malla
        Component_Mesh* meshComponent = newGameObject->GetComponent<Component_Mesh>();
        if (!meshComponent) {
            meshComponent = newGameObject->AddComponent<Component_Mesh>();
        }
        meshComponent->LoadMesh(aiMesh);

        // Obtener el material asociado a esta malla
        aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

        // Crear o obtener el componente de material
        Component_Material* materialComponent = newGameObject->GetComponent<Component_Material>();
        if (!materialComponent) {
            materialComponent = newGameObject->AddComponent<Component_Material>();
        }

        // Cargar texturas y propiedades del material
        LoadTexturesFromMaterial(material, materialComponent, scene, modelPath);
        LoadMaterialProperties(material, materialComponent);
    }

    // Recursión para procesar los hijos de este nodo
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        ImportarChilds(node->mChildren[i], scene, newGameObject, modelPath); // Llamada recursiva para los hijos
    }
}

void Importer::LoadTexturesFromMaterial(aiMaterial* material, Component_Material* materialComponent, const aiScene* scene, const std::string& modelPath) {
    aiString texturePath;

    // Obtener la ruta del directorio donde se encuentra el archivo FBX
    std::string scenePath = GetScenePath(modelPath);

    // Obtener la ruta de la textura difusa (si existe)
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePath);

        // Si la textura tiene un path vacío, es probable que sea una textura embebida
        if (texturePath.length == 0) {
            // Aquí verificamos si la textura está incrustada
            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str());

            if (embeddedTexture) {
                // La textura está incrustada, puedes procesarla aquí
                LoadEmbeddedTexture((aiTexture*)embeddedTexture, materialComponent);
            }
            else {
                std::cerr << "Error: No se pudo obtener la textura incrustada." << std::endl;
            }
        }
        else {
            // Si la ruta no está vacía, es una textura externa
            std::string textureFilePath = texturePath.C_Str();

            // Si la textura tiene una ruta relativa, combinamos con la ruta del archivo FBX
            if (textureFilePath[0] != '/' && textureFilePath[1] != ':') { // Si no es una ruta absoluta
                textureFilePath = scenePath + "/" + textureFilePath;
            }

            materialComponent->SetTexture(textureFilePath);  // Aquí debes utilizar la ruta completa de la textura
        }
    }
}

std::string Importer::GetScenePath(const std::string& modelPath) {
    fs::path p(modelPath);
    return p.parent_path().string();  // Devuelve el directorio donde se encuentra el archivo FBX
}

void Importer::LoadEmbeddedTexture(aiTexture* embeddedTexture, Component_Material* materialComponent) {
    // El contenido de la textura embebida está en embeddedTexture->pcData
    // Debes convertir los datos de la textura a un formato que puedas usar (por ejemplo, con una librería como stb_image o DevIL)

    if (embeddedTexture->mHeight == 0) {
        // Si la altura es 0, significa que es una imagen 2D
        GLuint textureID = 0;

        // Usamos stb_image para decodificar los datos de la textura embebida
        int width, height, channels;
        unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), embeddedTexture->mWidth, &width, &height, &channels, 0);

        if (data) {
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);

            // Asignamos la textura al componente de material
            materialComponent->SetTexture(textureID);
        }
        else {
            std::cerr << "Error al cargar la textura embebida." << std::endl;
        }
    }
}
void Importer::LoadMaterialProperties(aiMaterial* material, Component_Material* materialComponent) {
    aiColor4D diffuseColor;
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor)) {
        materialComponent->SetDiffuseColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
        materialComponent->SetDiffuseColor(1, 1, 1, 1);
    }

    // También puedes cargar otras propiedades como el color especular, si lo deseas
    aiColor4D specularColor;
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor)) {
        // Si tienes soporte para color especular, puedes agregarlo aquí
    }

    // Si tienes más propiedades (como reflectividad, opacidad, etc.), puedes extraerlas de la misma manera
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
