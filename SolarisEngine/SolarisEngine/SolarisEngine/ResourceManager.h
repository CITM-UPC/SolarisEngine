#pragma once
#include "UIDGen.h"
#include "Resource.h"
#include <map>

class ResourceManager
{
public:
	UID Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets);
	UID GenerateNewUID();
	const Resource* RequestResource(UID uid) const;
	Resource* RequestResource(UID uid);
	void ReleaseResource(UID uid);
private:
	Resource* CreateNewResource(const char* assetsFile, Resource::Type type);
private:
	std::map<UID, Resource*> resources;
};