#ifndef RESOURCETEXTURE_H
#define RESOURCETEXTURE_H

#include <string>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>

class ResourceTexture {
public:
    ResourceTexture() = default;
    ResourceTexture(const std::string& diffuse, const std::string& specular);

    // Métodos para serializar en formato de meta
    template <class Archive>
    void serialize(Archive& ar);

    // Métodos adicionales para cargar o procesar texturas
    void loadTexture();

private:
    std::string diffuse;
    std::string specular;
};

#endif // RESOURCETEXTURE_H
