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


	/*
	 std::string modelHash = IdentifierGenerator::GenerateHash(modelPath + GetNodeFullPath(node) + std::to_string(i));
        std::string modelHashFile = modelHash + ".mesh";
        std::string meshPath = (fs::path(MESHES_DIR) / modelHashFile).string();
        DataImporter::ExportData(meshPath, meshData);
        //meshComponent->LoadMesh(aiMesh);
        meshComponent->LoadMesh(modelHash);
	*/

	std::string fileName = UID + ".mesh";
	std::string meshPath = (fs::path(MESHES_DIR) / fileName).string();

	DataImporter::ImportarData(meshPath, meshData);

	vertices = meshData.vertices;
	indices = meshData.indices;
	normals = meshData.normals;
	faceNormals = meshData.faceNormals;
	texCoords = meshData.texCoords;


	// Simula la carga del mesh desde la biblioteca (Library/).
	//std::cout << "Loading mesh from library file: " << libraryFile << std::endl;

	// Aquí cargarías los datos de la malla desde el archivo en la librería.
	// A modo de ejemplo, podemos llenar los vectores con datos ficticios:

	// Cargar vértices de ejemplo
	//vertices.push_back({ {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} });
	//vertices.push_back({ {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} });
	//vertices.push_back({ {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f} });

	//// Cargar índices de ejemplo
	//indices = { 0, 1, 2 };

	//// Cargar normales de vértices de ejemplo
	//normals = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f };

	//// Cargar normales de caras de ejemplo
	//faceNormals = { 0.0f, 0.0f, 1.0f };


	return true; // Retorna true si la carga fue exitosa.
}
