#include "UIDGen.h"

// Genera un UUID como un arreglo de uint32_t
std::array<uint32_t, 4> UIDGen::GenerateUID() {
    std::array<uint32_t, 4> uid;
    for (size_t i = 0; i < uid.size(); ++i) {
        uid[i] = GenerateRandomUint32();
    }
    return uid;
}

// Genera un número aleatorio de 32 bits
uint32_t UIDGen::GenerateRandomUint32() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<uint32_t> distribution(0, 0xFFFFFFFF); // Rango de 32 bits
    return distribution(generator);
}
