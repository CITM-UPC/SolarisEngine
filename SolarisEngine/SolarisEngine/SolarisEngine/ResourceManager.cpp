#include "ResourceManager.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"  // Asegúrate de incluir todas las clases de recursos correspondientes
#include <iostream>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    // Al destruir el gestor, liberar todos los recursos cargados
    for (auto& pair : resources) {
        if (pair.second != nullptr) {
            pair.second->UnloadFromMemory();  // Llamada para descargar el recurso
            delete pair.second;              // Liberar memoria
        }
    }
}

Resource* ResourceManager::RequestResource(const std::string& uid, Resource::Type resourceType) {
    // Comprobar si el recurso ya está cargado
    auto it = resources.find(uid);
    if (it != resources.end()) {
        it->second->LoadToMemory();              // Carga en memoria
        it->second->IncrementReferenceCount();  // Aumentar la referencia
        return it->second;
    }

    // Si no está cargado, crearlo y cargarlo
    Resource* newResource = nullptr;

    switch (resourceType)
    {
    case Resource::UNKNOWN:
        break;
    case Resource::TEXTURE:
        break;
    case Resource::MATERIAL:
        break;
    case Resource::SPRITE:
        break;
    case Resource::MESH:
        newResource = new ResourceMesh(uid);
        break;
    case Resource::AUDIO:
        break;
    case Resource::SCENE:
        break;
    case Resource::BONE:
        break;
    case Resource::ANIMATION:
        break;
    default:
        newResource = nullptr;
        break;
    }

      // Aquí sería necesario crear el tipo correcto
    if (newResource && newResource->LoadToMemory()) {
        resources[uid] = newResource;
        return newResource;
    }

    // Si hubo un error al cargar
    std::cerr << "Error al cargar el recurso: " << uid << std::endl;
    delete newResource;
    return nullptr;
}

void ResourceManager::ReleaseResource(const std::string& uid) {
    auto it = resources.find(uid);
    if (it != resources.end()) {
        it->second->DecrementReferenceCount();
        if (it->second->GetReferenceCount() == 0) {
            it->second->UnloadFromMemory();
            delete it->second;
            resources.erase(it);
        }
    }
    else {
        std::cerr << "Recurso no encontrado para liberar: " << uid << std::endl;
    }
}

bool ResourceManager::IsResourceLoaded(const std::string& uid) const {
    return resources.find(uid) != resources.end();
}
