#ifndef RESOURCE_MESH_H
#define RESOURCE_MESH_H

#include "Resource.h"
#include <vector>
#include <string>
#include <cereal/cereal.hpp>
class ResourceMesh : public Resource {
public:
    // Constructor y destructor
ResourceMesh() : Resource() {} // Constructor por defecto
    ResourceMesh(std::string uid);
    ~ResourceMesh() override;

    // Métodos de carga y descarga de memoria
    bool LoadToMemory() override;
    void UnloadFromMemory() override;
    // Método de serialización
 /*   template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::base_class<Resource>(this), CEREAL_NVP(vertices), CEREAL_NVP(texCoords),
                CEREAL_NVP(indices), CEREAL_NVP(normals), CEREAL_NVP(faceNormals));
    }*/
    template <class Archive>
    void serialize(Archive& archive) {
        // Llamada a la función de la clase base
        archive(cereal::base_class<Resource>(this)); // Serializar el base class

        // Serializar los atributos específicos de ResourceMesh
        archive(CEREAL_NVP(vertices), CEREAL_NVP(texCoords), CEREAL_NVP(indices), CEREAL_NVP(normals), CEREAL_NVP(faceNormals));
    }


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
