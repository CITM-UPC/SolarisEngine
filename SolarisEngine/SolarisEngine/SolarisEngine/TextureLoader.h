#pragma once

#include "imgui.h"
#include <string>
#include <map>
#include <IL/il.h>
#include "stb_image.h"


#include <SDL2/SDL_opengl.h>

class TextureLoader {
public:
    TextureLoader();
    ~TextureLoader();

    ImTextureID LoadTexture(const std::string& filePath);
    void Cleanup();

    void GenerateMipmaps(GLuint textureID, int width, int height);

    ILuint LoadTextureDevIL(const std::string& filePath);
    ILuint LoadTextureDevIL(const std::string& filePath, int& width, int& height);

  

private:
    std::map<std::string, ImTextureID> textures; 
};
