#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>
#include <GL/glew.h> // Aseg�rate de incluir GLEW o tu librer�a de OpenGL preferida

#define ASSETS_DIR "./Assets"
#define LIBRARY_DIR "./Library"
#define MESHES_DIR "Library/Meshes"
#define MATERIALS_DIR "Library/Materials"
#define MODELS_DIR "Library/Models"

class Importer {
public:
    static Importer& getInstance() {
        static Importer instance;
        return instance;
    }

    Importer(const Importer&) = delete;
    void operator=(const Importer&) = delete;

    void Importar(const std::string& filepath);
    void Draw(const std::string& modelName);

private:
    Importer();
    void LoadMaterials(const aiScene* scene);
    void ProcessMeshes(const aiScene* scene);
    GLuint GetTextureIdForModel(const std::string& modelName);

    std::vector<std::string> materials;
    std::vector<std::string> meshes;
    std::map<std::string, GLuint> textureIds; // Mapear nombre de modelo a ID de textura
    // Aqu� puedes agregar estructuras para almacenar informaci�n de las mallas

    std::map<std::string, GLuint> vaos;
};
