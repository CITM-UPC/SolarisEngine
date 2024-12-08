#pragma once
#include "Meta.h"
#include <string>

class MetaManager {
public:
    static void CheckAndGenerateMetaFiles(const std::string& assetsDirectory);
    static Meta GenerateMeta(const std::string& sourcePath, const std::vector<std::shared_ptr<Resource>>& resources);
    static void SaveMeta(const Meta& meta, const std::string& metaFilePath);
    static Meta LoadMeta(const std::string& metaFilePath);
};
