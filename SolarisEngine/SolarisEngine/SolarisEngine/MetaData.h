#ifndef METADATA_H
#define METADATA_H

#include <string>
#include "ResourceTexture.h"
#include "ResourceMesh.h"

class MetaData {
public:
    MetaData() = default;
    MetaData(const std::string& resourcePath);

    // Métodos para establecer las opciones y recursos
    void SetUID(const std::string& uid);
    void SetImportOptions(bool flipNormals, float scale);

    // Convertir la clase ResourceTexture y ResourceMesh al formato adecuado para meta
    void SetTextures(const ResourceTexture& texture);
    void SetMesh(const ResourceMesh& mesh);

    // Guardar los datos en formato .meta
    void SaveMeta();

    // Cargar los datos desde un archivo .meta
    void LoadMeta();

private:
    std::string resourcePath;
    std::string uid;

    struct ImportOptions {
        bool flip_normals = false;
        float scale = 1.0f;

        template <class Archive>
        void serialize(Archive& ar);
    };

    ImportOptions importOptions;
    ResourceTexture texture; // Usamos ResourceTexture para manejar las texturas
    ResourceMesh mesh;       // Usamos ResourceMesh para manejar la malla
};

#endif // METADATA_H
