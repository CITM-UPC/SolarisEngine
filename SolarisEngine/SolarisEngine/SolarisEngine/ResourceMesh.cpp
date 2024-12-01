#include "ResourceMesh.h"
#include <iostream>
#include "App.h"
#include "string.h"
#include <fstream>

ResourceMesh::ResourceMesh(UID id)
    : Resource(id, Resource::texture)
{
}

ResourceMesh::~ResourceMesh() {}


bool ResourceMesh::LoadInMemory()
{
    // Aquí agregas la lógica para cargar la textura en memoria
    // Este es un ejemplo simple que solo marca como cargado
    std::cout << "Cargando textura " << GetAssetFile() << " en memoria" << std::endl;
    return true;  // Suponemos que la carga fue exitosa
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


