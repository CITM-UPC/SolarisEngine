#pragma once

#include "imgui.h"
#include <string>
#include <map>
#include <IL/il.h>

#include <SDL2/SDL_opengl.h>

class TextureLoader {
public:
    TextureLoader();
    ~TextureLoader();

    ImTextureID LoadTexture(const std::string& filePath);
    void Cleanup();

    void GenerateMipmaps(GLuint textureID, int width, int height);

    ILuint LoadTextureDevIL(const std::string& filePath);

  

private:
    std::map<std::string, ImTextureID> textures; 
};
