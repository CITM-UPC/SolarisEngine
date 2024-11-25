#include "ResourceManager.h"
#include "ResourceTexture.h"
#include "ResourceMesh.h"  // Asumiendo que tienes una clase ResourceMesh o similar

UID ResourceManager::Find(const char* file_in_assets) const
{
    for (const auto& entry : resources)
    {
        if (entry.second->GetAssetFile() == file_in_assets)
        {
            return entry.first;
        }
    }
    return UID(0); // Retorna un UID vacío si no se encuentra
}

UID ResourceManager::ImportFile(const char* new_file_in_assets)
{
    // Crear un nuevo recurso dependiendo del tipo de archivo
    Resource::Type type = Resource::unknown; // Aquí podrías agregar lógica para determinar el tipo
    Resource* newResource = CreateNewResource(new_file_in_assets, type);

    if (newResource)
    {
        UID newUID = newResource->GetUID();
        resources[newUID] = newResource;
        return newUID;
    }

    return UID(0); // Si no se pudo crear, devolver un UID vacío
}

UID ResourceManager::GenerateNewUID()
{
    UID currentUID = UID(UIDGen::GenerateRandomUint32());  // Comenzamos desde 1
    return currentUID;
}

const Resource* ResourceManager::RequestResource(UID uid) const
{
    auto it = resources.find(uid);
    if (it != resources.end())
    {
        return it->second;
    }
    return nullptr;
}

Resource* ResourceManager::RequestResource(UID uid)
{
    auto it = resources.find(uid);
    if (it != resources.end())                  
    {
        return it->second;
    }
    return nullptr;
}

void ResourceManager::ReleaseResource(UID uid)
{
    auto it = resources.find(uid);
    if (it != resources.end())
    {
        delete it->second;
        resources.erase(it);
    }
}

Resource* ResourceManager::CreateNewResource(const char* assetsFile, Resource::Type type)
{
    // Aquí puedes implementar lógica para crear diferentes tipos de recursos según el archivo
    switch (type)
    {
    case Resource::mesh:
        return new ResourceMesh(GenerateNewUID());
    case Resource::texture:
        return new ResourceTexture(GenerateNewUID());
        // Agregar casos para otros tipos de recursos (mesh, audio, etc.)
    default:
        return nullptr;
    }
}
