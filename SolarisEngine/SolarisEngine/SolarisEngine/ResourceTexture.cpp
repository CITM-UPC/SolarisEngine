#include "ResourceTexture.h"
#include <iostream>

ResourceTexture::ResourceTexture(const std::string& diffuse, const std::string& specular)
    : diffuse(diffuse), specular(specular) {}

template <class Archive>
void ResourceTexture::serialize(Archive& ar) {
    ar(CEREAL_NVP(diffuse), CEREAL_NVP(specular));
}

void ResourceTexture::loadTexture() {
    std::cout << "Cargando textura: " << diffuse << " y " << specular << std::endl;
}

// Esta definición es necesaria para instanciar el template de cereal
template void ResourceTexture::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
template void ResourceTexture::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
