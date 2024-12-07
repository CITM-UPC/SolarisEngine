#include "IdentifierGenerator.h"
#include <openssl/sha.h>
#include <random>
#include <sstream>
#include <iomanip>

std::string IdentifierGenerator::GenerateResourceID(const std::string& data) {
    return GenerateSHA256(data);
}

std::string IdentifierGenerator::GenerateHash(const std::string& data)
{
    return GenerateSHA256(data);
}

std::string IdentifierGenerator::GenerateEntityID() {
    return GenerateUUID();
}

uint32_t IdentifierGenerator::GenerateNetworkID() {
    static uint32_t counter = 0;
    return counter++;
}

std::string IdentifierGenerator::GenerateSHA256(const std::string& input) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), digest);

    std::ostringstream shaString;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        shaString << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return shaString.str();
}

std::string IdentifierGenerator::GenerateUUID() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<uint32_t> distribution(0, 0xFFFFFFFF);

    uint32_t data1 = distribution(generator);
    uint16_t data2 = static_cast<uint16_t>(distribution(generator));
    uint16_t data3 = static_cast<uint16_t>(distribution(generator) & 0x0FFF) | 0x4000; // UUID v4
    uint16_t data4 = static_cast<uint16_t>(distribution(generator) & 0x3FFF) | 0x8000; // Variant 1
    uint64_t data5 = (static_cast<uint64_t>(distribution(generator)) << 32) | distribution(generator);

    std::ostringstream uuid;
    uuid << std::hex << std::setw(8) << std::setfill('0') << data1 << "-"
        << std::setw(4) << data2 << "-"
        << std::setw(4) << data3 << "-"
        << std::setw(4) << data4 << "-"
        << std::setw(12) << data5;
    return uuid.str();
}
