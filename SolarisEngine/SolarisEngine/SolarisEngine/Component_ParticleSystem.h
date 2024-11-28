#ifndef __COMPONENT_PARTICLESYSTEM_H__
#define __COMPONENT_PARTICLESYSTEM_H__

#include "Component.h"
#include "Component_Material.h"
#include "Component_Billboard.h"
#include <vector>
#include <glm/glm.hpp>

class Component_Transform;

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float life;
    float maxLife;
    float size;
    glm::vec4 color;
    float rotation;
    float rotationSpeed;
};

class Component_ParticleSystem : public Component {
public:
    // Constructor y Destructor
    Component_ParticleSystem(GameObject* containerGO);
    ~Component_ParticleSystem();

    // M�todos de configuraci�n
    void SetEmitterProperties(float rate, float lifetime, glm::vec3 velocity, float size, glm::vec4 color);

    // M�todos de emisi�n y actualizaci�n de part�culas
    void EmitParticle();
    void UpdateParticles(double dt);
    void Update(double dt);

    // M�todos de dibujo
    void DrawComponent();
    void RenderParticle(const glm::vec3& position, float size, const glm::vec4& color, float rotation);

    // M�todo de duplicaci�n
    Component* Clone() const;

    // M�todos de habilitar/deshabilitar
    void Enable();
    void Disable();

    // Inspector para modificar propiedades
    void DrawInspectorComponent();

private:
    std::vector<Particle> particles;  // Lista de part�culas
    Component_Material* materialComponent;  // Componente de material (textura)

    // Propiedades de las part�culas
    float emissionRate;
    float particleLifetime;
    glm::vec3 particleVelocity;
    float particleSize;
    glm::vec4 particleColor;

    // Nuevas opciones de personalizaci�n
    glm::vec3 minVelocity; // Velocidad m�nima aleatoria
    glm::vec3 maxVelocity; // Velocidad m�xima aleatoria
    float minSize;         // Tama�o m�nimo aleatorio
    float maxSize;         // Tama�o m�ximo aleatorio
    float rotationSpeed;   // Velocidad de rotaci�n
    float minRotation;     // Rotaci�n m�nima aleatoria
    float maxRotation;     // Rotaci�n m�xima aleatoria
    glm::vec3 gravity;     // Gravedad personalizada
};

#endif // __COMPONENT_PARTICLESYSTEM_H__
