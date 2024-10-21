// Importer.h
#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <IL/il.h>

#define ASSETS_DIR "./Assets"
#define LIBRARY_DIR "./Library"
#define MESHES_DIR "Library/Meshes"
#define MATERIALS_DIR "Library/Materials"
#define MODELS_DIR "Library/Models"

class Importer {
public:
    static Importer& getInstance() {
        static Importer instance; // Instancia única
        return instance;
    }

    // Eliminar el constructor y el operador de asignación para evitar instanciación externa
    Importer(const Importer&) = delete;
    void operator=(const Importer&) = delete;

    void Importar(const std::string& filepath);

private:
    // Constructor privado
    Importer();

    void LoadMaterials(const aiScene* scene);
    void ProcessMeshes(const aiScene* scene);

    std::vector<std::string> materials; // Guardar materiales
    std::vector<std::string> meshes;     // Guardar mallas
};
