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
    // Aqu� agregas la l�gica para cargar la textura en memoria
    // Este es un ejemplo simple que solo marca como cargado
    std::cout << "Cargando textura " << GetAssetFile() << " en memoria" << std::endl;
    return true;  // Suponemos que la carga fue exitosa
}



void ResourceMesh::SaveMeshes()
{
    // Iterar sobre todas las mallas del componente
    auto* componentMesh = app->gameObject->GetComponent<Component_Mesh>();
    if (!componentMesh) {
        std::cerr << "Error: No se encontr� un componente de malla en el GameObject." << std::endl;
        return;
    }

    for (size_t i = 0; i < componentMesh->meshes.size(); i++) {
        Mesh& mesh = componentMesh->meshes[i];

        uint32_t numVertices = static_cast<uint32_t>(mesh.vertices.size() / 3);  // Cada v�rtice tiene 3 componentes: x, y, z
        uint32_t numIndices = static_cast<uint32_t>(mesh.indices.size());        // N�mero total de �ndices

        // Calcular el tama�o del buffer necesario
        uint32_t headerSize = sizeof(uint32_t) * 2;                // Cabecera (n�mero de �ndices y v�rtices)
        uint32_t indicesSize = sizeof(uint32_t) * numIndices;      // Tama�o de los �ndices
        uint32_t verticesSize = sizeof(float) * 3 * numVertices;   // Tama�o de los v�rtices
        uint32_t normalsSize = sizeof(float) * 3 * numVertices;    // Tama�o de las normales
        uint32_t uvSize = sizeof(float) * 2 * numVertices;         // Tama�o de las coordenadas UV

        uint32_t totalSize = headerSize + indicesSize + verticesSize + normalsSize + uvSize;

        // Crear el buffer
        char* fileBuffer = new char[totalSize];
        char* cursor = fileBuffer;

        // Escribir la cabecera
        uint32_t ranges[2] = { numIndices, numVertices };
        memcpy(cursor, ranges, headerSize);
        cursor += headerSize;

        // Escribir los �ndices
        memcpy(cursor, mesh.indices.data(), indicesSize);
        cursor += indicesSize;

        // Escribir los v�rtices
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


