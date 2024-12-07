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

    // M�todos de carga y descarga de memoria
    bool LoadToMemory() override;
    void UnloadFromMemory() override;

    std::vector<float> vertices;        // 1 v�rtice = {x, y, z, nx, ny, nz, u, v}
    std::vector<float> texCoords;      // Coordenadas de textura para cada v�rtice {u, v}
    std::vector<unsigned int> indices; // �ndices de la malla
    std::vector<float> normals;        // Normales de v�rtice
    std::vector<float> faceNormals;    // Normales por cara

private:
    // Funci�n auxiliar para cargar malla desde la biblioteca
    bool LoadMeshFromLibrary();


};

#endif // RESOURCE_MESH_H
