#pragma once
#include <vector>
#include <string>


struct MeshData {

    std::vector<float> vertices;      // Coordenadas de los v�rtices (x, y, z)
    std::vector<float> normals;       // Normales de los v�rtices (x, y, z)
    std::vector<float> faceNormals;       // Normales de los v�rtices (x, y, z)
    std::vector<float> texCoords;     // Coordenadas de textura (u, v)
    std::vector<unsigned int> indices; // �ndices de los v�rtices para formar tri�ngulos
};

struct TextureData {
    std::string diffuse;
    std::string specular;
};
