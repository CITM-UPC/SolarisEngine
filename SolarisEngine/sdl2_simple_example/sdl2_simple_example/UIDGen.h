#pragma once

#include <cstdint>
#include <array>
#include <random>

class UIDGen {
public:
    // Método estático para generar un UUID como un arreglo de uint32_t
    static std::array<uint32_t, 4> GenerateUID();
    static uint32_t GenerateRandomUint32();
};


    // Método para generar un número aleatorio de 32 bits
    
