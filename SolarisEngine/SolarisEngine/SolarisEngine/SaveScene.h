#ifndef SAVESCENE_H
#define SAVESCENE_H

#include <vector>
#include <fstream>
#include "SaveGameObject.h"

class SaveScene {
public:
    std::vector<SaveGameObject> saveGameObjects;

    void AddSaveGameObject(const SaveGameObject& obj);
    void Save(const std::string& filePath);
    void Load(const std::string& filePath);

    template<class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(saveGameObjects));
    }
};

#endif // SAVESCENE_H
#pragma once
