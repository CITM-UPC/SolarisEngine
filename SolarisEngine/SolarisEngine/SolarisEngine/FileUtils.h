#pragma once
#include <string>
#include <vector>
#include <filesystem>

class FileUtils {
public:
    static std::vector<std::string> GetAllFilesInDirectory(const std::string& directory, const std::string& extension = "");
    static bool FileExists(const std::string& filePath);
};
