#include "Resource.h"

Resource::Resource(std::string uid, Type type)
    : UID(uid), type(type), loadedToMemory(false), referenceCount(0) {}

Resource::~Resource() {}

std::string Resource::GetUID() const {
    return UID;
}

Resource::Type Resource::GetType() const {
    return type;
}

const std::string& Resource::GetAssetFile() const {
    return assetFile;
}

const std::string& Resource::GetLibraryFile() const {
    return libraryFile;
}

bool Resource::IsLoadedToMemory() const {
    return loadedToMemory;
}

uint Resource::GetReferenceCount() const {
    return referenceCount;
}

void Resource::IncrementReferenceCount() {
    ++referenceCount;
}

void Resource::DecrementReferenceCount() {
    if (referenceCount > 0) {
        --referenceCount;
    }
}

void Resource::UnloadFromMemory() {
    loadedToMemory = false;
}
