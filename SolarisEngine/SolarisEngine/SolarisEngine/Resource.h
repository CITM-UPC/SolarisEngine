#pragma once
#include "UIDGen.h"
#include "Defs.h"
#include <cereal/cereal.hpp>
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


	Resource() = default; // Constructor por defecto
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

	 //// Método de serialización
		//template <class Archive>
		//void serialize(Archive& archive) {
		//	archive(CEREAL_NVP(UID), CEREAL_NVP(type), CEREAL_NVP(assetFile), CEREAL_NVP(libraryFile));
		//}
	template <class Archive>
	void serialize(Archive& archive) {
		// Serializar los atributos comunes de Resource
		archive(CEREAL_NVP(UID), CEREAL_NVP(type), CEREAL_NVP(assetFile), CEREAL_NVP(libraryFile), CEREAL_NVP(loadedToMemory), CEREAL_NVP(referenceCount));
	}

	std::string UID;
	Type type = Type::UNKNOWN;


	std::string assetFile;  // Ruta a la carpeta Assets
	std::string libraryFile; // Ruta a la carpeta Library

	bool loadedToMemory;
	uint referenceCount;

protected:
	
};