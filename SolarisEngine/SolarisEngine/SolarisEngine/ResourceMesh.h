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

    // M�todos de carga y descarga de memoria
    bool LoadToMemory() override;
    void UnloadFromMemory() override;
    // M�todo de serializaci�n
 /*   template <class Archive>
    void serialize(Archive& archive) {
        archive(cereal::base_class<Resource>(this), CEREAL_NVP(vertices), CEREAL_NVP(texCoords),
                CEREAL_NVP(indices), CEREAL_NVP(normals), CEREAL_NVP(faceNormals));
    }*/
    template <class Archive>
    void serialize(Archive& archive) {
        // Llamada a la funci�n de la clase base
        archive(cereal::base_class<Resource>(this)); // Serializar el base class

        // Serializar los atributos espec�ficos de ResourceMesh
        archive(CEREAL_NVP(vertices), CEREAL_NVP(texCoords), CEREAL_NVP(indices), CEREAL_NVP(normals), CEREAL_NVP(faceNormals));
    }


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
