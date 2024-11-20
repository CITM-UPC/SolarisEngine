#pragma once

#include <cstdint>
#include <array>
#include <random>

struct UID {
    uint32_t uid;

    // Constructor que inicializa el valor de uid
    UID(uint32_t uid = 0) : uid(uid) {}

    // Sobrecargar el operador '<' para la comparación de dos objetos UID
    bool operator<(const UID& other) const {
        return uid < other.uid;  // Compara los valores internos `uid`
    }

    // Sobrecargar el operador '==' para comparación de igualdad
    bool operator==(const UID& other) const {
        return uid == other.uid;  // Compara los valores internos `uid`
    }

    // Sobrecargar el operador '!=' para comparación de desigualdad
    bool operator!=(const UID& other) const {
        return uid != other.uid;  // Compara los valores internos `uid`
    }
};

class UIDGen {
public:
    // Método estático para generar un UUID como un arreglo de uint32_t
    static std::array<uint32_t, 4> GenerateUID();
    static uint32_t GenerateRandomUint32();
};