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
    else {
        texturePath = filePath; // Guarda la ruta de la textura
      /*  textureWidth = app->textureLoader->GetTextureWidth(textureID);
        textureHeight = app->textureLoader->GetTextureHeight(textureID);*/
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
    if (!enabled) return;

    if (textureID != 0 && !showCheckerTexture) {
        glBindTexture(GL_TEXTURE_2D, textureID);  // Usa la textura cargada
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);  // Sin textura
        glColor3f(1.0f, 0.0f, 1.0f); // Color rosa para la textura de cuadros
        RenderCheckerPattern(); // Llama a una función para renderizar el patrón de cuadros
    }
}

void Component_Material::DrawInspectorComponent() {
    ImGui::Text("Material Properties");

    // Editor de color
    ImGui::ColorEdit3("Diffuse Color", diffuseColor);
    SetDiffuseColor(diffuseColor[0], diffuseColor[1], diffuseColor[2]);

    // Mostrar la textura si está cargada
    if (GetTextureID() != 0) {
        ImGui::Text("Texture:");
        ImGui::Text("Path: %s", texturePath.c_str());
        ImGui::Text("Size: %dx%d", textureWidth, textureHeight);

        // Renderizar la textura
        ImGui::Image((void*)(intptr_t)GetTextureID(), ImVec2(256, 256));
    }
    else {
        ImGui::Text("No texture loaded.");
    }

    // Checkbox para activar la textura de cuadros
    ImGui::Checkbox("Use Checker Texture", &showCheckerTexture);
}

void Component_Material::RenderCheckerPattern() {
    // Crea un patrón de cuadros en un pequeño buffer
    const int checkerSize = 64;
    GLubyte checkerImage[checkerSize][checkerSize][3];

    for (int i = 0; i < checkerSize; ++i) {
        for (int j = 0; j < checkerSize; ++j) {
            GLubyte c = (((i & 8) == 0) ^ ((j & 8) == 0)) * 255;
            checkerImage[i][j][0] = c;
            checkerImage[i][j][1] = c;
            checkerImage[i][j][2] = c;
        }
    }

    GLuint checkerTextureID;
    glGenTextures(1, &checkerTextureID);
    glBindTexture(GL_TEXTURE_2D, checkerTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, checkerSize, checkerSize, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerImage);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Usa la textura de cuadros en el objeto
    glBindTexture(GL_TEXTURE_2D, checkerTextureID);
}
