#pragma once
#include "Resource.h"
#include "ResourceMesh.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

// Registra el tipo base

CEREAL_REGISTER_TYPE(Resource)
//CEREAL_REGISTER_TYPE(std::shared_ptr<Resource>)

// Registra los tipos derivados
CEREAL_REGISTER_TYPE(ResourceMesh)

// Registra la relación entre el tipo base y los derivados
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, ResourceMesh)