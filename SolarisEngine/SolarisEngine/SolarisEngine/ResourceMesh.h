#pragma once

#include "Resource.h"

class ResourceMesh : public Resource
{



public:
	ResourceMesh(UID id); virtual
		~ResourceMesh(); bool LoadInMemory()
		override;
	/*void Save(Config& config) const override;
	void Load(const Config& config) override;*/
public:
	vec3 transformRotation;
	float importScale;

};