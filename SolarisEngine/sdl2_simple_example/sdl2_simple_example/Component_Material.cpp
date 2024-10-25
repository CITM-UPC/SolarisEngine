#include "Component_Material.h"
#include <iostream>


Component_Material::Component_Material(std::shared_ptr<GameObject> containerGO)
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
    if (!LoadTextureFromFile(filePath)) {
        std::cerr << "Error: No se pudo cargar la textura desde " << filePath << std::endl;
    }
}

bool Component_Material::LoadTextureFromFile(const std::string& filePath) {
    ilGenImages(1, &textureID);       // Genera un ID de textura con DevIL
    ilBindImage(textureID);

    if (ilLoadImage((const wchar_t*)filePath.c_str())) {  // Carga la imagen
        ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); // Convierte la imagen a RGBA

        // Crea la textura en OpenGL
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
            ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE,
            ilGetData());

        // Configura opciones de textura (filtrado y wrapping)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return true;
    }
    else {
        ilDeleteImages(1, &textureID); // Elimina el ID si falla la carga
        textureID = 0;
        return false;
    }
}

ILuint Component_Material::GetTextureID() const {
    return textureID;
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
            glColor3f(1.0f, 1.0f, 1.0f);             // Color blanco si hay textura
        }
        else {
            glBindTexture(GL_TEXTURE_2D, 0); // Desactiva la textura
            glColor3f(diffuseColor[0], diffuseColor[1], diffuseColor[2]); // Color difuso
        }
    }
}
