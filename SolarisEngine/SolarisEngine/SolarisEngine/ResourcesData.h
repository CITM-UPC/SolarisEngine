#pragma once
#include <vector>
#include <string>


struct MeshData {

    std::vector<float> vertices;      // Coordenadas de los vértices (x, y, z)
    std::vector<float> normals;       // Normales de los vértices (x, y, z)
    std::vector<float> faceNormals;       // Normales de los vértices (x, y, z)
    std::vector<float> texCoords;     // Coordenadas de textura (u, v)
    std::vector<unsigned int> indices; // Índices de los vértices para formar triángulos
};

struct TextureData {
    std::string diffuse;
    std::string specular;
};
