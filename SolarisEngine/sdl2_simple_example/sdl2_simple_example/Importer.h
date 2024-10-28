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
#include <GL/glew.h> // Asegúrate de incluir GLEW o tu librería de OpenGL preferida
#include "GameObject.h"
#include "Component.h"
#include "Defs.h"


class Importer {
public:
    static Importer& getInstance() {
        static Importer instance;
        return instance;
    }

    Importer(const Importer&) = delete;
    void operator=(const Importer&) = delete;

    void Draw(const std::string& modelName);
    void Draw(GameObject* gameObject); // Cambiado a puntero crudo
    GameObject* Importar(const std::string& modelPath); // Cambiado a puntero crudo
    GameObject* Importar(const std::string& modelPath, const std::string& texturePath); // Cambiado a puntero crudo

    unsigned int LoadTexture(const std::string& texturePath); // Declaración de LoadTexture

private:
    Importer();

    void LoadMaterials(const aiScene* scene);
    void ProcessMeshes(const aiScene* scene);
    GLuint GetTextureIdForModel(const std::string& modelName);

    std::vector<std::string> materials;
    std::map<std::string, GLuint> textureIds; // Mapear nombre de modelo a ID de textura
    std::map<std::string, GLuint> vaos;
};
