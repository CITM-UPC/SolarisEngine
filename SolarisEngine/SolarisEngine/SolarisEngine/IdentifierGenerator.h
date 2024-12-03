#pragma once
#ifndef IDENTIFIER_GENERATOR_H
#define IDENTIFIER_GENERATOR_H

#include <string>

class IdentifierGenerator {
public:
    // Genera un ID �nico basado en un hash MD5 del dato proporcionado
    static std::string GenerateResourceID(const std::string& data);
    static std::string GenerateHash(const std::string& data);

    // Genera un UUID v4 �nico
    static std::string GenerateEntityID();

    // Genera un ID incremental �nico para red
    static uint32_t GenerateNetworkID();

private:
    // Implementaci�n privada para calcular el hash MD5
    static std::string GenerateSHA256(const std::string& input);

    // Implementaci�n privada para generar un UUID v4
    static std::string GenerateUUID();
};

#endif // IDENTIFIER_GENERATOR_H

