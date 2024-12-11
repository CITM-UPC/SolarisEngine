#ifndef SAVESCENEMANAGER_H
#define SAVESCENEMANAGER_H

#include <string>
#include <map>
#include "SaveScene.h"

class SaveSceneManager {
public:
    void AddScene(const std::string& name, const SaveScene& scene);
    void Save(const std::string& name, const std::string& filePath);
    void Load(const std::string& name, const std::string& filePath);
    SaveScene& GetScene(const std::string& name);

private:
    std::map<std::string, SaveScene> scenes;
};

#endif // SAVESCENEMANAGER_H
