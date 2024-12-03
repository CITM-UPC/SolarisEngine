#include "ResourceMesh.h"
#include <iostream>

ResourceMesh::ResourceMesh(const std::string& path, int vertexCount, int indexCount)
    : path(path), vertex_count(vertexCount), index_count(indexCount) {}

template <class Archive>
void ResourceMesh::serialize(Archive& ar) {
    ar(CEREAL_NVP(path), CEREAL_NVP(vertex_count), CEREAL_NVP(index_count));
}

void ResourceMesh::loadMesh() {
    std::cout << "Cargando malla desde: " << path << " con " << vertex_count
        << " vértices y " << index_count << " índices." << std::endl;
}

// Esta definición es necesaria para instanciar el template de cereal
template void ResourceMesh::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
template void ResourceMesh::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
