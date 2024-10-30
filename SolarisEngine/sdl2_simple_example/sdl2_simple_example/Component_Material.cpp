#include "Component_Material.h"
#include "App.h"
#include <iostream>

Component_Material::Component_Material(GameObject* containerGO)  // Usar puntero crudo
    : Component(containerGO, ComponentType::Material), textureID(0) {
    diffuseColor[0] = 1.0f;  // Inicializa el color difuso a blanco
    diffuseColor[1] = 1.0f;
    diffuseColor[2] = 1.0f;
}

Component_Material::~Component_Material() {
    if (textureID != 0) {
        ilDeleteImages(1, &textureID);  // Libera la textura de DevIL si está cargada
    }
}

void Component_Material::SetDiffuseColor(float r, float g, float b) {
    diffuseColor[0] = r;
    diffuseColor[1] = g;
    diffuseColor[2] = b;
}

void Component_Material::SetTexture(const std::string& filePath) {

    textureID = app->textureLoader->LoadTextureDevIL(filePath);

    if (textureID == 0) {
        std::cerr << "Error: No se pudo cargar la textura desde " << filePath << std::endl;
    }
}

ILuint Component_Material::GetTextureID() const {
    return textureID;
}

glm::vec3 Component_Material::GetDiffuseColor() const
{
    return glm::vec3(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
}

void Component_Material::Enable() {
    enabled = true;
}

void Component_Material::Disable() {
    enabled = false;
}

void Component_Material::Update(double dt) {
    // Lógica de actualización si se necesita
}

void Component_Material::DrawComponent() {
    if (enabled) {
        if (textureID != 0) {
            glBindTexture(GL_TEXTURE_2D, textureID); // Activa la textura

            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            glColor3f(0.0f, 0.0f, 1.0f);             // Color blanco si hay textura
            //glColor3f(diffuseColor[0], diffuseColor[1], diffuseColor[2]); // Color difuso
        }
        else {
            glBindTexture(GL_TEXTURE_2D, 0); // Desactiva la textura
            glColor3f(1, diffuseColor[1], 1); // Color difuso
        }
    }
}
