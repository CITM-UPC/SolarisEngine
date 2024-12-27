#include "SaveGameObject.h"
#include <atomic>

// Usar una variable estática atómica para generar un ID único
std::atomic<int> nextUID(1);

int SaveGameObjectFactory::GenerateUniqueID() {
    return nextUID++;
}
