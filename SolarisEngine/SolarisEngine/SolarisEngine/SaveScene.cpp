#include "SaveScene.h"
#include <cereal/archives/json.hpp>

void SaveScene::AddSaveGameObject(const SaveGameObject& obj) {
    saveGameObjects.push_back(obj);
}

void SaveScene::Save(const std::string& filePath) {
    std::ofstream file(filePath);
    cereal::JSONOutputArchive archive(file);
    archive(cereal::make_nvp("SaveScene", *this));
}

void SaveScene::Load(const std::string& filePath) {
    std::ifstream file(filePath);
    cereal::JSONInputArchive archive(file);
    archive(cereal::make_nvp("SaveScene", *this));
}

