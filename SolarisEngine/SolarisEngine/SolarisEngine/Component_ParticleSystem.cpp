#include "Component_ParticleSystem.h"
#include "Component_Transform.h"
#include "App.h"
#include "CameraEditor.h"
#include "Debug.h"

Component_ParticleSystem::Component_ParticleSystem(GameObject* containerGO)
    : Component(containerGO, ComponentType::ParticleSystem),
    emissionRate(10.0f), particleLifetime(5.0f), particleVelocity(0.0f, 1.0f, 0.0f),
    particleSize(1.0f), particleColor(1.0f, 1.0f, 1.0f, 1.0f), materialComponent(nullptr) {

    // Intentar obtener el componente Material del GameObject asociado
    materialComponent = containerGO->GetComponent<Component_Material>();
}

Component_ParticleSystem::~Component_ParticleSystem() {
    particles.clear();
}

void Component_ParticleSystem::SetEmitterProperties(float rate, float lifetime, glm::vec3 velocity, float size, glm::vec4 color) {
    emissionRate = rate;
    particleLifetime = lifetime;
    particleVelocity = velocity;
    particleSize = size;
    particleColor = color;
}

void Component_ParticleSystem::EmitParticle() {
    Particle newParticle;

    // Configuración inicial de la partícula
    newParticle.position = containerGO->GetComponent<Component_Transform>()->GetPosition();
    newParticle.velocity = particleVelocity;  // Velocidad de emisión
    newParticle.acceleration = glm::vec3(0.0f, -9.81f, 0.0f);  // Fuerza de gravedad (opcional)
    newParticle.life = 0.0f;  // Empieza con vida en 0
    newParticle.maxLife = particleLifetime;
    newParticle.size = particleSize;
    newParticle.color = particleColor;
    newParticle.rotation = 0.0f;

    particles.push_back(newParticle);
}

void Component_ParticleSystem::UpdateParticles(double dt) {
    for (auto& particle : particles) {
        particle.velocity += particle.acceleration * static_cast<float>(dt);  // Actualiza la velocidad
        particle.position += particle.velocity * static_cast<float>(dt);      // Actualiza la posición
        particle.life += static_cast<float>(dt);                              // Incrementa la vida

        // Desaparecer si ha terminado su vida
        if (particle.life >= particle.maxLife) {
            particle = particles.back();
            particles.pop_back();
        }
    }
}

void Component_ParticleSystem::Update(double dt) {
    // Emitir partículas con base en la tasa de emisión
    static double timeSinceLastEmission = 0.0f;
    timeSinceLastEmission += dt;

    // Emite partículas si el tiempo desde la última emisión es mayor que la tasa de emisión
    while (timeSinceLastEmission >= 1.0f / emissionRate) {
        EmitParticle();
        timeSinceLastEmission -= 1.0f / emissionRate;
    }

    // Actualizar las partículas
    UpdateParticles(dt);
}

void Component_ParticleSystem::DrawComponent() {

   

    if (materialComponent && materialComponent->GetTextureID() != 0) {
        // Si existe el componente de material y tiene una textura cargada
        glBindTexture(GL_TEXTURE_2D, materialComponent->GetTextureID());  // Usar la textura del material
    }

    for (const auto& particle : particles) {
        RenderParticle(particle.position, particle.size, particle.color);
    }
}

void Component_ParticleSystem::RenderParticle(const glm::vec3& position, float size, const glm::vec4& color) {
    // Asegúrate de que la textura está activa
    if (materialComponent && materialComponent->GetTextureID() != 0) {
        glBindTexture(GL_TEXTURE_2D, materialComponent->GetTextureID());  // Usar la textura del material
    }

    // Coordenadas UV para el cuadrado de la partícula
    glm::vec2 uv1(0.0f, 0.0f);  // Inferior izquierda
    glm::vec2 uv2(1.0f, 0.0f);  // Inferior derecha
    glm::vec2 uv3(1.0f, 1.0f);  // Superior derecha
    glm::vec2 uv4(0.0f, 1.0f);  // Superior izquierda

    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);  // Traslación a la posición de la partícula
    glColor4f(color.r, color.g, color.b, color.a);  // Color de la partícula

    // Dibuja el cuadrado de la partícula, añadiendo UVs para el mapeo de la textura
    glBegin(GL_QUADS);

    // Vértice 1: Inferior izquierdo
    glTexCoord2f(uv1.x, uv1.y);
    glVertex3f(-size, -size, 0);

    // Vértice 2: Inferior derecho
    glTexCoord2f(uv2.x, uv2.y);
    glVertex3f(size, -size, 0);

    // Vértice 3: Superior derecho
    glTexCoord2f(uv3.x, uv3.y);
    glVertex3f(size, size, 0);

    // Vértice 4: Superior izquierdo
    glTexCoord2f(uv4.x, uv4.y);
    glVertex3f(-size, size, 0);

    glEnd();

    glPopMatrix();
}

void Component_ParticleSystem::DrawInspectorComponent() {
    if (ImGui::CollapsingHeader("Particle System Settings")) {
        ImGui::SliderFloat("Emission Rate", &emissionRate, 0.0f, 100.0f);
        ImGui::SliderFloat("Particle Lifetime", &particleLifetime, 0.1f, 10.0f);
        ImGui::InputFloat3("Particle Velocity", &particleVelocity[0]);
        ImGui::SliderFloat("Particle Size", &particleSize, 0.1f, 10.0f);
        ImGui::ColorEdit4("Particle Color", &particleColor[0]);
    }
}


Component* Component_ParticleSystem::Clone() const {
    return new Component_ParticleSystem(*this);
}

void Component_ParticleSystem::Enable() {
    enabled = true;
}

void Component_ParticleSystem::Disable() {
    enabled = false;
}
