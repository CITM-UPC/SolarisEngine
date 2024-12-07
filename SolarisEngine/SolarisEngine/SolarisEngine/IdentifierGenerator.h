#pragma once
#ifndef IDENTIFIER_GENERATOR_H
#define IDENTIFIER_GENERATOR_H

#include <string>

class IdentifierGenerator {
public:
    // Genera un ID único basado en un hash MD5 del dato proporcionado
    static std::string GenerateResourceID(const std::string& data);
    static std::string GenerateHash(const std::string& data);

    // Genera un UUID v4 único
    static std::string GenerateEntityID();

    // Genera un ID incremental único para red
    static uint32_t GenerateNetworkID();

private:
    // Implementación privada para calcular el hash MD5
    static std::string GenerateSHA256(const std::string& input);

    // Implementación privada para generar un UUID v4
    static std::string GenerateUUID();
};

#endif // IDENTIFIER_GENERATOR_H

