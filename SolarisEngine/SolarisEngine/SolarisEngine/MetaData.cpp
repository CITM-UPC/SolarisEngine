#include "MetaData.h"
#include <iostream>
#include <fstream>
#include <cereal/archives/json.hpp>

MetaData::MetaData(const std::string& resourcePath)
    : resourcePath(resourcePath) {}

void MetaData::SetUID(const std::string& uid) {
    this->uid = uid;
}

void MetaData::SetImportOptions(bool flipNormals, float scale) {
    importOptions.flip_normals = flipNormals;
    importOptions.scale = scale;
}

void MetaData::SetTextures(const ResourceTexture& texture) {
    this->texture = texture;
}

void MetaData::SetMesh(const ResourceMesh& mesh) {
    this->mesh = mesh;
}

void MetaData::SaveMeta() {
    std::ofstream os(resourcePath + ".meta");
    if (os.is_open()) {
        cereal::JSONOutputArchive archive(os);
        archive(CEREAL_NVP(uid), CEREAL_NVP(importOptions), CEREAL_NVP(texture), CEREAL_NVP(mesh));
        std::cout << "Archivo .meta guardado: " << resourcePath << ".meta" << std::endl;
    }
    else {
        std::cerr << "Error al guardar el archivo .meta" << std::endl;
    }
}

void MetaData::LoadMeta() {
    std::ifstream is(resourcePath + ".meta");
    if (is.is_open()) {
        cereal::JSONInputArchive archive(is);
        archive(CEREAL_NVP(uid), CEREAL_NVP(importOptions), CEREAL_NVP(texture), CEREAL_NVP(mesh));
        std::cout << "Archivo .meta cargado: " << resourcePath << ".meta" << std::endl;
    }
    else {
        std::cerr << "Error al cargar el archivo .meta" << std::endl;
    }
}

template <class Archive>
void MetaData::ImportOptions::serialize(Archive& ar) {
    ar(CEREAL_NVP(flip_normals), CEREAL_NVP(scale));
}
