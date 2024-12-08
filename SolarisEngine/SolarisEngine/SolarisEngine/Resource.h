#pragma once
#include "UIDGen.h"
#include "Defs.h"
class Resource
{


public:

	enum Type {
		UNKNOWN = 0,
		TEXTURE,
		MATERIAL,
		SPRITE,
		MESH,
		AUDIO,
		SCENE,
		BONE,
		ANIMATION
	};



	Resource(std::string uid, Resource::Type type);
	virtual ~Resource();

	std::string GetUID() const;

	Resource::Type GetType() const;
	
	const std::string& GetAssetFile() const;
	const std::string& GetLibraryFile() const;

	bool IsLoadedToMemory() const;
	virtual bool LoadToMemory() = 0; // Cada tipus de recurs implementa la seva càrrega
	virtual void UnloadFromMemory();

	uint GetReferenceCount() const;
	void IncrementReferenceCount();
	void DecrementReferenceCount();

	std::string UID;
	Type type = Type::UNKNOWN;


	std::string assetFile;  // Ruta a la carpeta Assets
	std::string libraryFile; // Ruta a la carpeta Library

	bool loadedToMemory;
	uint referenceCount;

protected:
	
};