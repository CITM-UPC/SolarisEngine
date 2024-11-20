#ifndef EDITOR_SAVE_LOAD_H
#define EDITOR_SAVE_LOAD_H

#include <string>
#include "Component_Mesh.h"
#include "Component_Material.h"

class EditorSaveLoad {
public:
    // Save functions
    static bool SaveMesh(const Mesh& mesh, const std::string& filePath);
    static bool SaveMaterial(const Material& material, const std::string& filePath);
    static bool SaveTexture(const std::string& texturePath, const std::string& savePath);

    // Load functions
    static bool LoadMesh(Mesh& mesh, const std::string& filePath);
    static bool LoadMaterial(Material& material, const std::string& filePath);
    static bool LoadTexture(const std::string& texturePath, std::string& outSavePath);
    static bool SaveTextureDevIL(Material* material, const std::string& fileName);

private:
    
    // Helper for DevIL texture saving
    static bool SaveDDS(const std::string& texturePath, const std::string& savePath);
};

#endif // EDITOR_SAVE_LOAD_H
