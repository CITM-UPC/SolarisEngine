#include "FileUtils.h"
#include <filesystem>

std::vector<std::string> FileUtils::GetAllFilesInDirectory(const std::string& directory, const std::string& extension) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            if (extension.empty() || entry.path().extension() == extension) {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}

bool FileUtils::FileExists(const std::string& filePath) {
    return std::filesystem::exists(filePath);
}
