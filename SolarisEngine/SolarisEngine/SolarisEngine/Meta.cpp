#include "Meta.h"

Meta::Meta() {}

Meta::Meta(const std::string& fileUUID, const std::string& sourcePath)
    : fileUUID(fileUUID), sourcePath(sourcePath) {}

void Meta::AddResource(std::shared_ptr<Resource> resource) {
    resources.push_back(resource);
}

const std::vector<std::shared_ptr<Resource>>& Meta::GetResources() const {
    return resources;
}

const std::string& Meta::GetFileUUID() const {
    return fileUUID;
}

const std::string& Meta::GetSourcePath() const {
    return sourcePath;
}
