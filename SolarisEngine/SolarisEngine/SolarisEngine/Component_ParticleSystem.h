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

    // Métodos de configuración
    void SetEmitterProperties(float rate, float lifetime, glm::vec3 velocity, float size, glm::vec4 color);

    // Métodos de emisión y actualización de partículas
    void EmitParticle();
    void UpdateParticles(double dt);
    void Update(double dt);

    // Métodos de dibujo
    void DrawComponent();
    void RenderParticle(const glm::vec3& position, float size, const glm::vec4& color, float rotation);

    // Método de duplicación
    Component* Clone() const;

    // Métodos de habilitar/deshabilitar
    void Enable();
    void Disable();

    // Inspector para modificar propiedades
    void DrawInspectorComponent();

private:
    std::vector<Particle> particles;  // Lista de partículas
    Component_Material* materialComponent;  // Componente de material (textura)

    // Propiedades de las partículas
    float emissionRate;
    float particleLifetime;
    glm::vec3 particleVelocity;
    float particleSize;
    glm::vec4 particleColor;

    // Nuevas opciones de personalización
    glm::vec3 minVelocity; // Velocidad mínima aleatoria
    glm::vec3 maxVelocity; // Velocidad máxima aleatoria
    float minSize;         // Tamaño mínimo aleatorio
    float maxSize;         // Tamaño máximo aleatorio
    float rotationSpeed;   // Velocidad de rotación
    float minRotation;     // Rotación mínima aleatoria
    float maxRotation;     // Rotación máxima aleatoria
    glm::vec3 gravity;     // Gravedad personalizada
};

#endif // __COMPONENT_PARTICLESYSTEM_H__
