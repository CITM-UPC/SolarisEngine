#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Resource.h"
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/json.hpp>

class Meta {
public:
    Meta();
    Meta(const std::string& fileUUID, const std::string& sourcePath);

    void AddResource(std::shared_ptr<Resource> resource);
    const std::vector<std::shared_ptr<Resource>>& GetResources() const;

    const std::string& GetFileUUID() const;
    const std::string& GetSourcePath() const;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(fileUUID), CEREAL_NVP(sourcePath), CEREAL_NVP(resources));
    }

private:
    std::string fileUUID;  // UID único para el archivo fuente
    std::string sourcePath; // Ruta al archivo fuente
    std::vector<std::shared_ptr<Resource>> resources; // Recursos asociados
};