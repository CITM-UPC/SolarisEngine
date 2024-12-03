#ifndef RESOURCEMESH_H
#define RESOURCEMESH_H

#include <string>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>

class ResourceMesh {
public:
    ResourceMesh() = default;
    ResourceMesh(const std::string& path, int vertexCount, int indexCount);

    // Métodos para serializar en formato de meta
    template <class Archive>
    void serialize(Archive& ar);

    // Métodos adicionales para cargar o procesar la malla
    void loadMesh();

private:
    std::string path;
    int vertex_count = 0;
    int index_count = 0;
};

#endif // RESOURCEMESH_H
