#pragma once
#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "Defs.h"
#include <string>
#include <IL/il.h>  // Biblioteca DevIL para cargar texturas
#include <GL/glew.h>
#include <gl/GL.h>
#include "TextureLoader.h"


struct Material {
    ILuint textureID;
    float diffuseColor[3];
    int textureWidth = 0;
    int textureHeight = 0;
    std::string texturePath;
};


class Component_Material : public Component {
public:
    // Constructor y Destructor
    Component_Material(GameObject* containerGO); // Usar puntero crudo
    ~Component_Material();

    // Métodos de configuración de material
    void SetDiffuseColor(float r, float g, float b);
    void SetTexture(const std::string& filePath);

    ILuint GetTextureID();

    // Métodos de gestión
    void Enable() override;
    void Disable() override;
    void Update(double dt) override;
    void DrawComponent() override;
    void DrawInspectorComponent() override;

   

  

    glm::vec3 GetDiffuseColor() const;


    
    bool showCheckerTexture = false; // Para habilitar la textura de cuadros

private:

    void CreateCheckerPattern();

    // Propiedades del material
    //float diffuseColor[3]; // Color difuso del material (RGB)
    //ILuint textureID;      // ID de la textura de DevIL
    ILuint textureCheckersID;      // ID de la textura de DevIL
    Material* material;
};

#endif // !__COMPONENT_MATERIAL_H__
