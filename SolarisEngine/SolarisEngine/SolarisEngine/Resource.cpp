#include "Resource.h"
#include <iostream>

Resource::Resource(UID uid, Resource::Type type)
    : uid(uid), type(type)
{
}

Resource::~Resource() {}

Resource::Type Resource::GetType() const
{
    return type;
}

UID Resource::GetUID() const
{
    return uid;
}

const char* Resource::GetAssetFile() const
{
    return assetsFile.c_str();
}

const char* Resource::GetLibraryFile() const
{
    return Libraryile.c_str();
}

bool Resource::IsLoadedToMemory() const
{
    // Aqu� puedes agregar l�gica para saber si el recurso est� cargado en memoria
    return false;  // Por ahora, siempre retorna false como ejemplo
}

bool Resource::LoadToMemory()
{
    // L�gica para cargar a memoria si es necesario
    return false;  // Por ahora, retorna false como ejemplo
}

uint Resource::GetReferenceCount() const
{
    return referenceCount;
}

bool Resource::IsLoadedInMemory()
{
    return false;
}
