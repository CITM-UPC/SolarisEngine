#include "ResourceMesh.h"
#include "ResourceManager.h" // Asegúrate de incluir esto o el equivalente si tienes un administrador de recursos
#include <fstream> 
namespace fs = std::filesystem;

ResourceMesh::ResourceMesh(std::string uid) : Resource(uid, Type::MESH) {
	// Inicialización de cualquier cosa adicional, si es necesario
}

ResourceMesh::~ResourceMesh() {
	// Destruir cualquier recurso asignado dinámicamente, si es necesario
}

bool ResourceMesh::LoadToMemory() {
	// Aquí deberíamos cargar el archivo de la malla, generalmente desde un archivo en la biblioteca

	// Ejemplo de llamada para cargar la malla desde la biblioteca
	return LoadMeshFromLibrary();
}

void ResourceMesh::UnloadFromMemory() {
	// Liberar los recursos de memoria cuando ya no sean necesarios
	vertices.clear();
	texCoords.clear();
	indices.clear();
	normals.clear();
	faceNormals.clear();
}



bool ResourceMesh::LoadMeshFromLibrary() {

	MeshData meshData;

	std::string fileName = UID + ".mesh";
	std::string meshPath = (fs::path(MESHES_DIR) / fileName).string();

	DataImporter::ImportarData(meshPath, meshData);

	vertices = meshData.vertices;
	indices = meshData.indices;
	normals = meshData.normals;
	faceNormals = meshData.faceNormals;
	texCoords = meshData.texCoords;



	return true; // Retorna true si la carga fue exitosa.
}
