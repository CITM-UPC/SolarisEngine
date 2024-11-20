#include "EditorSaveLoad.h"
#include <fstream>
#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

// ------------ Save Functions ------------

bool EditorSaveLoad::SaveMesh(const Mesh& mesh, const std::string& filePath) {
    uint header[3] = {
        (uint)mesh.vertices.size(),
        (uint)mesh.indices.size(),
        (uint)mesh.normals.size()
    };

    size_t size = sizeof(header) +
        mesh.vertices.size() * sizeof(float) +
        mesh.texCoords.size() * sizeof(float) +
        mesh.indices.size() * sizeof(unsigned int) +
        mesh.normals.size() * sizeof(float) +
        mesh.faceNormals.size() * sizeof(float);

    char* buffer = new char[size];
    char* cursor = buffer;

    // Header
    memcpy(cursor, header, sizeof(header));
    cursor += sizeof(header);

    // Vertices
    memcpy(cursor, mesh.vertices.data(), mesh.vertices.size() * sizeof(float));
    cursor += mesh.vertices.size() * sizeof(float);

    // Texture coordinates
    memcpy(cursor, mesh.texCoords.data(), mesh.texCoords.size() * sizeof(float));
    cursor += mesh.texCoords.size() * sizeof(float);

    // Indices
    memcpy(cursor, mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));
    cursor += mesh.indices.size() * sizeof(unsigned int);

    // Normals
    memcpy(cursor, mesh.normals.data(), mesh.normals.size() * sizeof(float));
    cursor += mesh.normals.size() * sizeof(float);

    // Face normals
    memcpy(cursor, mesh.faceNormals.data(), mesh.faceNormals.size() * sizeof(float));
    cursor += mesh.faceNormals.size() * sizeof(float);

    // Write to file
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save mesh to " << filePath << std::endl;
        delete[] buffer;
        return false;
    }
    file.write(buffer, size);
    file.close();

    delete[] buffer;
    return true;
}

bool EditorSaveLoad::SaveMaterial(const Material& material, const std::string& filePath) {
    size_t size = sizeof(material.textureID) +
        sizeof(material.diffuseColor) +
        sizeof(int) * 2 + // textureWidth, textureHeight
        material.texturePath.size() + 1;

    char* buffer = new char[size];
    char* cursor = buffer;

    // Texture ID
    memcpy(cursor, &material.textureID, sizeof(material.textureID));
    cursor += sizeof(material.textureID);

    // Diffuse color
    memcpy(cursor, material.diffuseColor, sizeof(material.diffuseColor));
    cursor += sizeof(material.diffuseColor);

    // Texture dimensions
    memcpy(cursor, &material.textureWidth, sizeof(int));
    cursor += sizeof(int);
    memcpy(cursor, &material.textureHeight, sizeof(int));
    cursor += sizeof(int);

    // Texture path
    strcpy_s(cursor, material.texturePath.size() + 1, material.texturePath.c_str());
    cursor += material.texturePath.size() + 1;

    // Write to file
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to save material to " << filePath << std::endl;
        delete[] buffer;
        return false;
    }
    file.write(buffer, size);
    file.close();

    delete[] buffer;
    return true;
}

bool EditorSaveLoad::SaveTexture(const std::string& texturePath, const std::string& savePath) {
    return SaveDDS(texturePath, savePath);
}


bool EditorSaveLoad::SaveTextureDevIL(Material* material, const std::string& fileName) {
    
    
    std::string savePath = std::string(TEXTURES_DIR) + "/" + fileName;

    if (material->textureID == 0) {
        return false; // Verifica que haya una textura cargada
    }

    // Cargar la textura activa en DevIL
    ilBindImage(material->textureID);



    // Obtener datos de la textura desde memoria
    ILubyte* data = ilGetData();
    if (!data) {
        return false; // Si no hay datos, no se puede guardar
    }

    // Configurar formato para guardar (ejemplo DDS)
    ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // DXT5 es un buen formato comprimido

    // Guardar la textura al archivo
    if (!ilSaveImage((const wchar_t*)savePath.c_str())) {
        return false; // Si no puede guardar, retorna error
    }

    return true; // Textura guardada con éxito
}

bool EditorSaveLoad::SaveDDS(const std::string& texturePath, const std::string& savePath) {
    ilInit();
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage((const wchar_t*)texturePath.c_str())) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        ilDeleteImages(1, &imageID);
        return false;
    }

    ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // Set compression format
    if (!ilSaveImage((const wchar_t*)savePath.c_str())) {
        std::cerr << "Failed to save texture to DDS: " << savePath << std::endl;
        ilDeleteImages(1, &imageID);
        return false;
    }

    ilDeleteImages(1, &imageID);
    return true;
}

// ------------ Load Functions ------------

bool EditorSaveLoad::LoadMesh(Mesh& mesh, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to load mesh from " << filePath << std::endl;
        return false;
    }

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();

    char* cursor = buffer;

    uint header[3];
    memcpy(header, cursor, sizeof(header));
    cursor += sizeof(header);

    mesh.vertices.resize(header[0]);
    memcpy(mesh.vertices.data(), cursor, mesh.vertices.size() * sizeof(float));
    cursor += mesh.vertices.size() * sizeof(float);

    mesh.indices.resize(header[1]);
    memcpy(mesh.indices.data(), cursor, mesh.indices.size() * sizeof(unsigned int));
    cursor += mesh.indices.size() * sizeof(unsigned int);

    mesh.normals.resize(header[2]);
    memcpy(mesh.normals.data(), cursor, mesh.normals.size() * sizeof(float));
    cursor += mesh.normals.size() * sizeof(float);

    delete[] buffer;
    return true;
}

bool EditorSaveLoad::LoadMaterial(Material& material, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to load material from " << filePath << std::endl;
        return false;
    }

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size];
    file.read(buffer, size);
    file.close();

    char* cursor = buffer;

    memcpy(&material.textureID, cursor, sizeof(material.textureID));
    cursor += sizeof(material.textureID);

    memcpy(material.diffuseColor, cursor, sizeof(material.diffuseColor));
    cursor += sizeof(material.diffuseColor);

    memcpy(&material.textureWidth, cursor, sizeof(int));
    cursor += sizeof(int);

    memcpy(&material.textureHeight, cursor, sizeof(int));
    cursor += sizeof(int);

    material.texturePath = std::string(cursor);

    delete[] buffer;
    return true;
}

bool EditorSaveLoad::LoadTexture(const std::string& texturePath, std::string& outSavePath) {
    outSavePath = texturePath + ".dds";
    return SaveDDS(texturePath, outSavePath);
}