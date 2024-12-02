#pragma once

#include "Resource.h"
#include "Component_Mesh.h"

class ResourceMesh : public Resource
{



public:
	ResourceMesh(UID id); virtual
		~ResourceMesh();
	void SetMeshData(const Mesh& mesh);
	bool LoadInMemory()
		override;
	void LoadMeshes(const std::string& filePath, Mesh& mesh);
	void SaveMeshes();
	void LoadFromLibrary(const std::string& filePath);

	bool IsLoadedInMemory() override;
	/*void Save(Config& config) const override;
	void Load(const Config& config) override;*/

	void SetTransformRotation(const vec3& rotation);
	void SetImportScale(float scale);

	vec3 GetTransformRotation() const;
	float GetImportScale() const;
public:
	Mesh meshData;
	vec3 transformRotation;
	float importScale;
	bool isLoaded = false;

};