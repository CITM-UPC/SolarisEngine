#pragma once

#include <cstdint>
#include <array>
#include <random>

class UIDGen {
public:
    // M�todo est�tico para generar un UUID como un arreglo de uint32_t
    static std::array<uint32_t, 4> GenerateUID();
    static uint32_t GenerateRandomUint32();
};


    // M�todo para generar un n�mero aleatorio de 32 bits
    
