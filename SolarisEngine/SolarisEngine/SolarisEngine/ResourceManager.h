#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>
#include "ResourcesData.h"
#include "DataImporter.h"
#include "Resource.h"
#include "ResourceMesh.h"  // Asegúrate de incluir otras clases de recursos (como ResourceTexture, etc.)

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Solicitar un recurso por UID
    Resource* RequestResource(const std::string& uid, Resource::Type resourceType);

    // Liberar un recurso por UID
    void ReleaseResource(const std::string& uid);

    // Verificar si un recurso está cargado
    bool IsResourceLoaded(const std::string& uid) const;

private:
    std::unordered_map<std::string, Resource*> resources;  // Mapa de recursos cargados
};

#endif // RESOURCE_MANAGER_H
