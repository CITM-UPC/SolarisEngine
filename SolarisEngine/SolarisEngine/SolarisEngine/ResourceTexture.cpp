#include "ResourceTexture.h"
#include <iostream>

ResourceTexture::ResourceTexture(UID id)
    : Resource(id, Resource::texture)
{
}

ResourceTexture::~ResourceTexture() {}

bool ResourceTexture::LoadInMemory()
{
    // Aquí agregas la lógica para cargar la textura en memoria
    // Este es un ejemplo simple que solo marca como cargado
    std::cout << "Cargando textura " << GetAssetFile() << " en memoria" << std::endl;
    return true;  // Suponemos que la carga fue exitosa
}
