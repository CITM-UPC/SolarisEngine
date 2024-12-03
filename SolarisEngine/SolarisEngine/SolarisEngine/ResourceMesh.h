#ifndef RESOURCE_MESH_H
#define RESOURCE_MESH_H

#include "Resource.h"
#include <vector>
#include <string>

class ResourceMesh : public Resource {
public:
    // Constructor y destructor
    ResourceMesh(std::string uid);
    ~ResourceMesh() override;

    // Métodos de carga y descarga de memoria
    bool LoadToMemory() override;
    void UnloadFromMemory() override;

    std::vector<float> vertices;        // 1 vértice = {x, y, z, nx, ny, nz, u, v}
    std::vector<float> texCoords;      // Coordenadas de textura para cada vértice {u, v}
    std::vector<unsigned int> indices; // Índices de la malla
    std::vector<float> normals;        // Normales de vértice
    std::vector<float> faceNormals;    // Normales por cara

private:
    // Función auxiliar para cargar malla desde la biblioteca
    bool LoadMeshFromLibrary();


};

#endif // RESOURCE_MESH_H
