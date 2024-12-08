#include "MetaManager.h"
#include <fstream>
#include <cereal/archives/json.hpp>
#include "IdentifierGenerator.h" // Tu generador de UIDs
#include "FileUtils.h"
#include "ResourceMesh.h"

//#include "ResourceRegistration.cpp"




void MetaManager::CheckAndGenerateMetaFiles(const std::string& assetsDirectory)
{
    auto assetFiles = FileUtils::GetAllFilesInDirectory(assetsDirectory);

    for (const auto& filePath : assetFiles) {

        if (std::filesystem::path(filePath).extension() == ".meta") {
            continue;
        }

        std::string metaFilePath = filePath + ".meta";

        if (!FileUtils::FileExists(metaFilePath)) {
            std::cout << "Generating .meta for: " << filePath << std::endl;

            std::vector<std::shared_ptr<Resource>> resources;

            auto extension = std::filesystem::path(filePath).extension();
            if (extension == ".fbx") {
                std::shared_ptr<ResourceMesh> mesh = std::make_shared<ResourceMesh>(IdentifierGenerator::GenerateEntityID());
                mesh->assetFile = filePath;
                mesh->libraryFile = "Library/" + mesh->GetUID() + ".mesh";
                resources.push_back(mesh);
            }
            // Aquí puedes añadir soporte para más tipos de recursos

            Meta meta = MetaManager::GenerateMeta(filePath, resources);
            MetaManager::SaveMeta(meta, metaFilePath);
        }
    }
}

Meta MetaManager::GenerateMeta(const std::string& sourcePath, const std::vector<std::shared_ptr<Resource>>& resources) {
    std::string fileUUID = IdentifierGenerator::GenerateEntityID(); // Usa tu función de generación de UUID
    Meta meta(fileUUID, sourcePath);

    for (const auto& resource : resources) {
        meta.AddResource(resource);
    }

    return meta;
}

void MetaManager::SaveMeta(const Meta& meta, const std::string& metaFilePath) {
    std::ofstream file(metaFilePath);
    if (file.is_open()) {
        cereal::JSONOutputArchive archive(file);
        archive(CEREAL_NVP(meta));
    }
}

Meta MetaManager::LoadMeta(const std::string& metaFilePath) {
    std::ifstream file(metaFilePath);
    Meta meta;
    if (file.is_open()) {
        cereal::JSONInputArchive archive(file);
        archive(CEREAL_NVP(meta));
    }
    return meta;
}
