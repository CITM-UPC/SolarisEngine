#pragma once

#include "Resource.h"
#include "Component_Mesh.h"

class ResourceMesh : public Resource
{



public:
	ResourceMesh(UID id); virtual
		~ResourceMesh(); bool LoadInMemory()
		override;
	void LoadMeshes(const std::string& filePath, Mesh& mesh);
	void SaveMeshes();
	/*void Save(Config& config) const override;
	void Load(const Config& config) override;*/
public:
	vec3 transformRotation;
	float importScale;

};