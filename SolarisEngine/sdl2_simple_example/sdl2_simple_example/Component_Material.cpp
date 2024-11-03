#include "Component_Material.h"
#include "App.h"
#include "TextureLoader.h"
#include <iostream>

Component_Material::Component_Material(GameObject* containerGO)  // Usar puntero crudo
    : Component(containerGO, ComponentType::Material) {

    material = new Material();

    material->textureID = 0;

    material->diffuseColor[0] = 1.0f;  // Inicializa el color difuso a blanco
    material->diffuseColor[1] = 1.0f;
    material->diffuseColor[2] = 1.0f;

}

Component_Material::~Component_Material() {
    if (material->textureID != 0) {
        ilDeleteImages(1, &material->textureID);  // Libera la textura de DevIL si está cargada
    }
}

void Component_Material::SetDiffuseColor(float r, float g, float b) {
   material->diffuseColor[0] = r;
   material->diffuseColor[1] = g;
   material->diffuseColor[2] = b;
}

void Component_Material::SetTexture(const std::string& filePath) {

    if (!material) {
        material = new Material();
    }


    material->textureID = app->textureLoader->LoadTextureDevIL(filePath, material->textureWidth, material->textureHeight);

    if (material->textureID == 0) {
        std::cerr << "Error: No se pudo cargar la textura desde " << filePath << std::endl;
    }
    else {
        material->texturePath = filePath; // Guarda la ruta de la textura
      /*  textureWidth = app->textureLoader->GetTextureWidth(textureID);
        textureHeight = app->textureLoader->GetTextureHeight(textureID);*/
    }
}

ILuint Component_Material::GetTextureID() {
    

    if (showCheckerTexture) {

        if (textureCheckersID == 0) {
            CreateCheckerPattern();
        }

        return textureCheckersID;
    }
    return material->textureID;
}

glm::vec3 Component_Material::GetDiffuseColor() const
{
    return glm::vec3(material->diffuseColor[0], material->diffuseColor[1], material->diffuseColor[2]);
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

    //Creo k no sirve
    if (material->textureID != 0 && !showCheckerTexture) {
        glBindTexture(GL_TEXTURE_2D, material->textureID);  // Usa la textura cargada
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(material->diffuseColor[0], material->diffuseColor[1], material->diffuseColor[2]);
    }
    else {
     
        glBindTexture(GL_TEXTURE_2D, textureCheckersID);
        glColor3f(1.0f, 0.0f, 1.0f); // Color rosa para la textura de cuadros
       
    }
}

void Component_Material::DrawInspectorComponent() {


    if (ImGui::CollapsingHeader(u8"\ue08F Material")) {
        // Editor de color
        ImGui::ColorEdit3("Diffuse Color", material->diffuseColor);
        SetDiffuseColor(material->diffuseColor[0], material->diffuseColor[1], material->diffuseColor[2]);

        // Mostrar la textura si está cargada
        if (GetTextureID() != 0) {
            ImGui::Text("Texture:");

            ImGui::TextWrapped("Path: %s", material->texturePath.c_str());
            ImGui::Text("Size: %dx%d", material->textureWidth, material->textureHeight);
        

            // Renderizar la textura
            ImGui::Image((void*)(intptr_t)GetTextureID(), ImVec2(256, 256));
        }
        else {
            ImGui::Text("No texture loaded.");
        }

        // Checkbox para activar la textura de cuadros
        ImGui::Checkbox("Use Checker Texture", &showCheckerTexture);
    
    }

    
}

Component* Component_Material::Clone() const
{
    return new Component_Material(*this);
}

void Component_Material::CreateCheckerPattern() {
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

    ;
    glGenTextures(1, &textureCheckersID);
    glBindTexture(GL_TEXTURE_2D, textureCheckersID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, checkerSize, checkerSize, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerImage);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Usa la textura de cuadros en el objeto
    glBindTexture(GL_TEXTURE_2D, textureCheckersID);
}
