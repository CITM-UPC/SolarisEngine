#include "SaveSceneManager.h"

void SaveSceneManager::AddScene(const std::string& name, const SaveScene& scene) {
    scenes[name] = scene;
}

void SaveSceneManager::Save(const std::string& name, const std::string& filePath) {
    if (scenes.find(name) != scenes.end()) {
        scenes[name].Save(filePath);
    }
}

void SaveSceneManager::Load(const std::string& name, const std::string& filePath) {
    SaveScene scene;
    scene.Load(filePath);
    scenes[name] = scene;
}

SaveScene& SaveSceneManager::GetScene(const std::string& name) {
    return scenes.at(name);
}
