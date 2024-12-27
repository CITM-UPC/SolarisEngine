#ifndef __SAVEGAMEOBJECT_H__
#define __SAVEGAMEOBJECT_H__

#include <string>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>

// Estructura del componente
struct ComponentSaveGameObject {
    int type;
    std::string path;
    std::vector<float> tint;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(type), CEREAL_NVP(path), CEREAL_NVP(tint));
    }
};

// Estructura del objeto de juego guardado
struct SaveGameObject {
    int uid;
    int parentUid;
    std::string name;
    std::vector<float> translation;
    std::vector<float> scale;
    std::vector<float> rotation;
    std::vector<ComponentSaveGameObject> components;

    template<class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(uid), CEREAL_NVP(parentUid), CEREAL_NVP(name),
            CEREAL_NVP(translation), CEREAL_NVP(scale), CEREAL_NVP(rotation),
            CEREAL_NVP(components));
    }
};

class SaveGameObjectFactory {
public:
    static int GenerateUniqueID();
};

#endif // SAVEGAMEOBJECT_H
