#include "ResourceMesh.h"
#include <iostream>
#include "App.h"
#include "string.h"
#include <fstream>

ResourceMesh::ResourceMesh(UID id)
    : Resource(id, Resource::mesh), importScale(1.0f), transformRotation(vec3(0.0f, 0.0f, 0.0f)) {
}


ResourceMesh::~ResourceMesh() {}

void ResourceMesh::SetMeshData(const Mesh& mesh) {
    this->meshData = mesh; // Asigna la malla recibida a `meshData`
}


bool ResourceMesh::LoadInMemory() {
    if (!isLoaded) {
        // Lógica para cargar los datos en memoria
        // Por ejemplo, cargar desde un archivo en `Library`:
        std::string filePath = "Library/Meshes/mesh_" + std::to_string(GetUID().uid) + ".mesh";
        LoadFromLibrary(filePath);

        // Actualiza el estado a true si la carga fue exitosa
        isLoaded = true;
        std::cout << "Malla cargada en memoria: UID " << GetUID().uid << std::endl;
        return true;
    }
    return false; // Si ya estaba cargada, no vuelve a cargar
}


bool ResourceMesh::IsLoadedInMemory() {
    return isLoaded;
}



void ResourceMesh::SaveMeshes()
{
    // Iterar sobre todas las mallas del componente
    auto* componentMesh = app->gameObject->GetComponent<Component_Mesh>();
    if (!componentMesh) {
        std::cerr << "Error: No se encontró un componente de malla en el GameObject." << std::endl;
        return;
    }

    for (size_t i = 0; i < componentMesh->meshes.size(); i++) {
        Mesh& mesh = componentMesh->meshes[i];

        uint32_t numVertices = static_cast<uint32_t>(mesh.vertices.size() / 3);  // Cada vértice tiene 3 componentes: x, y, z
        uint32_t numIndices = static_cast<uint32_t>(mesh.indices.size());        // Número total de índices

        // Calcular el tamaño del buffer necesario
        uint32_t headerSize = sizeof(uint32_t) * 2;                // Cabecera (número de índices y vértices)
        uint32_t indicesSize = sizeof(uint32_t) * numIndices;      // Tamaño de los índices
        uint32_t verticesSize = sizeof(float) * 3 * numVertices;   // Tamaño de los vértices
        uint32_t normalsSize = sizeof(float) * 3 * numVertices;    // Tamaño de las normales
        uint32_t uvSize = sizeof(float) * 2 * numVertices;         // Tamaño de las coordenadas UV

        uint32_t totalSize = headerSize + indicesSize + verticesSize + normalsSize + uvSize;

        // Crear el buffer
        char* fileBuffer = new char[totalSize];
        char* cursor = fileBuffer;

        // Escribir la cabecera
        uint32_t ranges[2] = { numIndices, numVertices };
        memcpy(cursor, ranges, headerSize);
        cursor += headerSize;

        // Escribir los índices
        memcpy(cursor, mesh.indices.data(), indicesSize);
        cursor += indicesSize;

        // Escribir los vértices
        memcpy(cursor, mesh.vertices.data(), verticesSize);
        cursor += verticesSize;

        // Escribir las normales
        memcpy(cursor, mesh.normals.data(), normalsSize);
        cursor += normalsSize;

        // Escribir las coordenadas UV
        memcpy(cursor, mesh.texCoords.data(), uvSize);
        cursor += uvSize;

        // Crear el archivo binario
        std::string filePath = "Library/Meshes/mesh_" + std::to_string(i) + "_" + std::to_string(GetUID().uid) + ".mesh";

        std::ofstream outFile(filePath, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(fileBuffer, totalSize);
            outFile.close();
            std::cout << "Malla " << i << " guardada exitosamente en " << filePath << std::endl;
        }
        else {
            std::cerr << "Error al guardar la malla " << i << "." << std::endl;
        }

        // Liberar el buffer
        delete[] fileBuffer;
    }


}

void ResourceMesh::LoadFromLibrary(const std::string& filePath) {
    std::ifstream inFile(filePath, std::ios::binary);
    if (inFile.is_open()) {
        uint32_t ranges[2];
        inFile.read(reinterpret_cast<char*>(ranges), sizeof(ranges));
        uint32_t numIndices = ranges[0];
        uint32_t numVertices = ranges[1];

        meshData.indices.resize(numIndices);
        inFile.read(reinterpret_cast<char*>(meshData.indices.data()), numIndices * sizeof(uint32_t));

        meshData.vertices.resize(numVertices * 3);
        inFile.read(reinterpret_cast<char*>(meshData.vertices.data()), numVertices * 3 * sizeof(float));

        meshData.normals.resize(numVertices * 3);
        inFile.read(reinterpret_cast<char*>(meshData.normals.data()), numVertices * 3 * sizeof(float));

        meshData.texCoords.resize(numVertices * 2);
        inFile.read(reinterpret_cast<char*>(meshData.texCoords.data()), numVertices * 2 * sizeof(float));

        inFile.close();
        std::cout << "Malla cargada exitosamente desde " << filePath << std::endl;
    }
    else {
        std::cerr << "Error al cargar la malla desde " << filePath << std::endl;
    }
}

void ResourceMesh::SetTransformRotation(const vec3& rotation) {
    this->transformRotation = rotation;
}

void ResourceMesh::SetImportScale(float scale) {
    this->importScale = scale;
}

vec3 ResourceMesh::GetTransformRotation() const {
    return transformRotation;
}

float ResourceMesh::GetImportScale() const {
    return importScale;
}


