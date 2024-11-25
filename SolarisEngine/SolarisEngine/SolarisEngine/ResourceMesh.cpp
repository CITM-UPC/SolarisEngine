#include "ResourceMesh.h"
#include <iostream>
#include "App.h"

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


//void ResourceMesh::SaveMeshes()
//{
//    // Aseg�rate de que tienes acceso a los datos de la malla (�ndices, v�rtices, normales, etc.)
//    // Por ejemplo:
//    // mesh.indices, mesh.vertices, mesh.normals, mesh.uvs (dependiendo de tu estructura)
//
//    // Supongamos que tienes las siguientes variables (estos deber�an ser miembros de la clase ResourceMesh o estar definidos de alguna manera)
//    const uint32_t numVertices = app->gameObject->GetComponent().mesh;
//    const uint32_t numIndices = mesh.
//
//    // Asignamos suficiente espacio para almacenar todos los datos (�ndices + v�rtices + normales + UVs, etc.)
//    uint32_t size = sizeof(uint32_t) * 2  // Para almacenar la cantidad de �ndices y v�rtices
//        + sizeof(float) * 3 * numVertices  // Coordenadas de los v�rtices (3 valores por v�rtice)
//        + sizeof(float) * 3 * numVertices  // Normales (3 valores por v�rtice)
//        + sizeof(float) * 2 * numVertices;  // Coordenadas UV (2 valores por v�rtice, si aplican)
//
//    char* fileBuffer = new char[size];
//    char* cursor = fileBuffer;
//
//    // Guardar la cantidad de �ndices y v�rtices (primeros 8 bytes)
//    uint32_t ranges[2] = { numIndices, numVertices };
//    memcpy(cursor, ranges, sizeof(ranges));
//    cursor += sizeof(ranges);
//
//    // Guardar los �ndices (suponiendo que est�n almacenados en mesh.indices como un arreglo de uint32_t)
//    memcpy(cursor, mesh.indices, sizeof(uint32_t) * numIndices);
//    cursor += sizeof(uint32_t) * numIndices;
//
//    // Guardar los v�rtices (suponiendo que mesh.vertices contiene los datos de los v�rtices como un arreglo de float[3])
//    memcpy(cursor, mesh.vertices, sizeof(float) * 3 * numVertices);  // 3 valores por v�rtice (x, y, z)
//    cursor += sizeof(float) * 3 * numVertices;
//
//    // Guardar las normales (suponiendo que mesh.normals contiene los datos de las normales como un arreglo de float[3])
//    memcpy(cursor, mesh.normals, sizeof(float) * 3 * numVertices);  // 3 valores por normal
//    cursor += sizeof(float) * 3 * numVertices;
//
//    // Guardar las coordenadas UV (suponiendo que mesh.uvs contiene los datos como un arreglo de float[2])
//    memcpy(cursor, mesh.uvs, sizeof(float) * 2 * numVertices);  // 2 valores por coordenada UV (u, v)
//    cursor += sizeof(float) * 2 * numVertices;
//
//    // Guardar los datos en un archivo binario
//    std::ofstream outFile("Library/Meshes/mesh_" + std::to_string(GetUID()) + ".mesh", std::ios::binary);
//    if (outFile.is_open())
//    {
//        outFile.write(fileBuffer, size);  // Escribe el buffer en el archivo
//        outFile.close();
//        std::cout << "Malla guardada exitosamente." << std::endl;
//    }
//    else
//    {
//        std::cerr << "Error al guardar la malla." << std::endl;
//    }
//
//    // Liberar la memoria del buffer
//    delete[] fileBuffer;
//}
