#pragma once
#include "ResourcesData.h"
#include "Resource.h"
#include <string>
#include "App.h"

// "INTERFAZ"
class DataImporter
{


public:


	//Mesh
	static void TransformData(const aiMesh* ai_mesh, MeshData& meshData);
	static void ExportData(const std::string& filePath, const MeshData& meshData);
	static void ImportarData(const std::string& filePath, MeshData& meshData);

	

	//Texture
	//static void TransformData(uint textureID, TextureData& textureData);
	//static void ExportData(const std::string& filePath, const TextureData& meshData);
	//static void ImportarData(const std::string& filePath, TextureData& textureData);



private:
	static void CalculateFaceNormals(MeshData& mesh);

};