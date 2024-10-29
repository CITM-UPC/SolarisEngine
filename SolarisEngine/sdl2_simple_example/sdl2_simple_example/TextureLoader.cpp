#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"
#include "stb_image.h"
#include "imgui.h"


TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
    Cleanup();
}

ImTextureID TextureLoader::LoadTexture(const std::string& filePath) {
    if (textures.find(filePath) != textures.end()) {
        return textures[filePath];
    }

    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (!data) {
        return nullptr;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 生成 Mipmaps
    //glGenerateMipmap(GL_TEXTURE_2D);
    // 如果不工作，可以使用手动生成 Mipmaps 的函数
    GenerateMipmaps(textureID, width, height);

    stbi_image_free(data);

    textures[filePath] = (ImTextureID)(uintptr_t)textureID;

    return textures[filePath];
}



void TextureLoader::Cleanup() {
    for (auto& pair : textures) {
        GLuint textureID = (GLuint)(uintptr_t)pair.second;
        glDeleteTextures(1, &textureID);
    }
    textures.clear();
}


void TextureLoader::GenerateMipmaps(GLuint textureID, int width, int height) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    for (int level = 1; level < 5; level++) { // 假设最大 Mipmap 层级为 5
        width /= 2;
        height /= 2;
        if (width <= 0 || height <= 0) break;

        glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glCopyTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, 0, 0, width, height, 0);
    }
}
