#pragma once
#include "UIDGen.h"
#include "Defs.h"
class Resource
{


public:

	enum Type {
		texture,
		mesh, audio,
		scene, bone,
		animation,
		unknown
	};



	Resource(UID uid, Resource::Type type);
	virtual ~Resource();
	Resource::Type GetType() const; UID
		GetUID() const;
	const char* GetAssetFile() const; const
		char* GetLibraryFile() const; bool
		IsLoadedToMemory() const;
	bool LoadToMemory();
	uint GetReferenceCount() const;
	/*virtual void Save(Config& config) const;
	virtual void Load(const Config& config);*/
	virtual bool LoadInMemory() = 0;
	virtual bool IsLoadedInMemory();
protected:
	UID uid = UID(0);
	std::string assetsFile;
	std::string Libraryile;
	Type type = unknown;
	uint referenceCount = 0;
};