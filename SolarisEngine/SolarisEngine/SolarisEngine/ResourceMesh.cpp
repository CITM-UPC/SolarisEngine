#include "ResourceMesh.h"
#include <iostream>

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
