#include "Component_BoundingBox.h"
#include "Component_Transform.h"
#include "App.h"
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>



Component_BoundingBox::Component_BoundingBox(GameObject* containerGO)
    : Component(containerGO, ComponentType::BoundingBox),
    minLocal(glm::vec3(0.0f)), maxLocal(glm::vec3(0.0f)) {}

Component_BoundingBox::~Component_BoundingBox() {}

void Component_BoundingBox::Enable() {
    enabled = true;
}

void Component_BoundingBox::Disable() {
    enabled = false;
}

void Component_BoundingBox::Update(double dt) {
    // Aquí puede ir lógica adicional si necesitas hacer actualizaciones cada frame
}

void Component_BoundingBox::DrawComponent() {
    if (!enabled) return;

    // Obtener el transform del GameObject
    auto transform = containerGO->GetComponent<Component_Transform>();
    if (!transform) {
        std::cerr << "Error: No se pudo obtener el componente de transformación." << std::endl;
        return;
    }
    glm::mat4 modelMatrix = transform->GetModelMatrix();

    // Aplicar la matriz de transformación en el modelo
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(modelMatrix));

    // Definir las esquinas de la bounding box en el espacio mundial
    glm::vec3 corners[8] = {
        glm::vec3(minWorld.x, minWorld.y, minWorld.z),  // V0
        glm::vec3(maxWorld.x, minWorld.y, minWorld.z),  // V1
        glm::vec3(minWorld.x, maxWorld.y, minWorld.z),  // V2
        glm::vec3(maxWorld.x, maxWorld.y, minWorld.z),  // V3
        glm::vec3(minWorld.x, minWorld.y, maxWorld.z),  // V4
        glm::vec3(maxWorld.x, minWorld.y, maxWorld.z),  // V5
        glm::vec3(minWorld.x, maxWorld.y, maxWorld.z),  // V6
        glm::vec3(maxWorld.x, maxWorld.y, maxWorld.z)   // V7
    };

    // Establecer el color de la bounding box
    glColor3f(1.0f, 1.0f, 0.0f);  // Color amarillo

    // Dibujar las 12 aristas de la bounding box usando líneas
    glBegin(GL_LINES);
    // Parte inferior (aristas del plano `minWorld.z`)
    glVertex3fv(glm::value_ptr(corners[0])); glVertex3fv(glm::value_ptr(corners[1])); // V0 ↔ V1
    glVertex3fv(glm::value_ptr(corners[1])); glVertex3fv(glm::value_ptr(corners[3])); // V1 ↔ V3
    glVertex3fv(glm::value_ptr(corners[3])); glVertex3fv(glm::value_ptr(corners[2])); // V3 ↔ V2
    glVertex3fv(glm::value_ptr(corners[2])); glVertex3fv(glm::value_ptr(corners[0])); // V2 ↔ V0

    // Parte superior (aristas del plano `maxWorld.z`)
    glVertex3fv(glm::value_ptr(corners[4])); glVertex3fv(glm::value_ptr(corners[5])); // V4 ↔ V5
    glVertex3fv(glm::value_ptr(corners[5])); glVertex3fv(glm::value_ptr(corners[7])); // V5 ↔ V7
    glVertex3fv(glm::value_ptr(corners[7])); glVertex3fv(glm::value_ptr(corners[6])); // V7 ↔ V6
    glVertex3fv(glm::value_ptr(corners[6])); glVertex3fv(glm::value_ptr(corners[4])); // V6 ↔ V4

    // Conexiones verticales (vértices inferiores a los superiores)
    glVertex3fv(glm::value_ptr(corners[0])); glVertex3fv(glm::value_ptr(corners[4])); // V0 ↔ V4
    glVertex3fv(glm::value_ptr(corners[1])); glVertex3fv(glm::value_ptr(corners[5])); // V1 ↔ V5
    glVertex3fv(glm::value_ptr(corners[2])); glVertex3fv(glm::value_ptr(corners[6])); // V2 ↔ V6
    glVertex3fv(glm::value_ptr(corners[3])); glVertex3fv(glm::value_ptr(corners[7])); // V3 ↔ V7
    glEnd();

    glPopMatrix(); // Restablecer la matriz después de dibujar
}




void Component_BoundingBox::UpdateBoundingBox(const glm::vec3& minLocal, const glm::vec3& maxLocal, const Component_Transform& transform) {
    this->minLocal = minLocal;
    this->maxLocal = maxLocal;

    // Obten la matriz de transformación mundial
    glm::mat4 modelMatrix = transform.GetModelMatrix();

    // Calcula las esquinas de la bounding box en espacio mundial
    glm::vec3 corners[8] = {
        modelMatrix * glm::vec4(minLocal.x, minLocal.y, minLocal.z, 1.0f),
        modelMatrix * glm::vec4(maxLocal.x, minLocal.y, minLocal.z, 1.0f),
        modelMatrix * glm::vec4(minLocal.x, maxLocal.y, minLocal.z, 1.0f),
        modelMatrix * glm::vec4(maxLocal.x, maxLocal.y, minLocal.z, 1.0f),
        modelMatrix * glm::vec4(minLocal.x, minLocal.y, maxLocal.z, 1.0f),
        modelMatrix * glm::vec4(maxLocal.x, minLocal.y, maxLocal.z, 1.0f),
        modelMatrix * glm::vec4(minLocal.x, maxLocal.y, maxLocal.z, 1.0f),
        modelMatrix * glm::vec4(maxLocal.x, maxLocal.y, maxLocal.z, 1.0f)
    };

    // Calcula el AABB en el espacio mundial usando las esquinas transformadas
    minWorld = maxWorld = corners[0];
    for (int i = 1; i < 8; ++i) {
        minWorld = glm::min(minWorld, corners[i]);
        maxWorld = glm::max(maxWorld, corners[i]);
    }
}

const glm::vec3& Component_BoundingBox::GetMinPointWorld() const {
    return minWorld;
}

const glm::vec3& Component_BoundingBox::GetMaxPointWorld() const {
    return maxWorld;
}

//bool Component_BoundingBox::IsInsideFrustum(const Frustum& frustum) const {
//    // Aquí iría la lógica para verificar si el AABB está dentro del frustum.
//    // Este es un lugar para una función como frustum.CheckAABB(minWorld, maxWorld);
//    return frustum.CheckAABB(minWorld, maxWorld);
//}
