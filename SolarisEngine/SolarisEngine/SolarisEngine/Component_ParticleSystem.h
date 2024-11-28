#pragma once
#ifndef COMPONENT_PARTICLESYSTEM_H
#define COMPONENT_PARTICLESYSTEM_H

#include "Component.h"
#include "Component_Material.h"
#include <vector>
#include "glm/glm.hpp"

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float life;           // Tiempo de vida
    float maxLife;        // Vida m�xima (para controlar la desaparici�n)
    float size;           // Tama�o de la part�cula
    glm::vec4 color;      // Color de la part�cula
    float rotation;       // Rotaci�n de la part�cula
};

class Component_ParticleSystem : public Component {
public:
    // Constructor y Destructor
    Component_ParticleSystem(GameObject* containerGO);
    ~Component_ParticleSystem() override;

    // M�todos para actualizar y renderizar el sistema
    void Update(double dt) override;
    void DrawComponent() override;

    // M�todos para configurar el sistema de part�culas
    void SetEmitterProperties(float rate, float lifetime, glm::vec3 velocity, float size, glm::vec4 color);

    // Para el inspector (si lo necesitas)
    void DrawInspectorComponent() override;


    Component* Clone() const override;
    void Enable() override;
    void Disable() override;

private:
    // Configuraci�n del sistema de part�culas
    float emissionRate; // Part�culas generadas por segundo
    float particleLifetime;
    glm::vec3 particleVelocity;
    float particleSize;
    glm::vec4 particleColor;

    // Datos de las part�culas
    std::vector<Particle> particles;
    Component_Material* materialComponent;  // Referencia al material del GameObject

    // M�todos internos
    void EmitParticle();
    void UpdateParticles(double dt);
    void RenderParticle(const glm::vec3& position, float size, const glm::vec4& color);
};

#endif // COMPONENT_PARTICLESYSTEM_H
