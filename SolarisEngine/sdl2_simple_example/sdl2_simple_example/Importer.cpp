#include "Importer.h"
#include "GameObject.h"
#include "Component_Mesh.h" // Asegúrate de que tienes esta clase definida
#include <memory> // Para std::shared_ptr

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
//
//std::shared_ptr<GameObject> Importer::Importar(const std::string& filepath) {
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
//
//    if (!scene) {
//        std::cerr << "Error al cargar el archivo: " << importer.GetErrorString() << std::endl;
//        return nullptr;
//    }
//
//    std::cout << "Archivo cargado exitosamente: " << filepath << std::endl;
//
//    LoadMaterials(scene);
//
//    auto gameObject = std::make_shared<GameObject>("ImportedFBX");
//
//    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
//        aiMesh* ai_mesh = scene->mMeshes[i];
//        std::string meshName = ai_mesh->mName.C_Str();
//        //meshes.push_back(meshName);
//
//        // Crear y agregar un componente de malla al GameObject
//        auto meshComponent = std::make_unique<Component_Mesh>(gameObject);
//        meshComponent->LoadMesh(ai_mesh); // Implementa esta función en Component_Mesh
//        gameObject->AddComponent<Component_Mesh>(std::move(meshComponent));
//
//        std::cout << "Malla procesada: " << meshName << " con " << ai_mesh->mNumVertices << " vértices." << std::endl;
//    }
//
//    return gameObject; // Devuelve el GameObject creado
//}

std::shared_ptr<GameObject> Importer::Importar(const std::string& filePath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    auto newGameObject = std::make_shared<GameObject>(scene->mName.C_Str());
    newGameObject->AddComponent<Component_Mesh>();

    // Recorrer todos los nodos de la escena y extraer los datos del modelo
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = scene->mMeshes[i];

        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);
        }

        for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j) {
            const aiFace& face = aiMesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                mesh.indices.push_back(face.mIndices[k]);
            }
        }

        newGameObject->GetComponent<Component_Mesh>()->meshes.push_back(mesh);
    }
    return newGameObject;
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

void Importer::Draw(const std::shared_ptr<GameObject>& gameObject) {
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
}
