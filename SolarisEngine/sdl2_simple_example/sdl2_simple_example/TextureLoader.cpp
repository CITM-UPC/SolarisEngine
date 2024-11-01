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
        printf("Failed to load image from path: %s\n", filePath.c_str());
        return nullptr;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);


    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


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

ILuint TextureLoader::LoadTextureDevIL(const std::string& filePath) {
    
    ILuint textureID;
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
    }
    else {
        ilDeleteImages(1, &textureID); // Elimina el ID si falla la carga
        textureID = 0;
    }

    return textureID;
}

ILuint TextureLoader::LoadTextureDevIL(const std::string& filePath, int& width, int& height) {
    ILuint imageID;
    ilGenImages(1, &imageID);       // Genera un ID de textura en DevIL
    ilBindImage(imageID);

    // Cargar la imagen
    if (!ilLoadImage((const wchar_t*)filePath.c_str())) {
        ilDeleteImages(1, &imageID); // Eliminar imagen si la carga falla
        return 0;
    }

    // Obtener las dimensiones de la imagen cargada
    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);

    // Convertir la imagen a un formato compatible con OpenGL
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Generar una textura en OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Cargar los datos de la imagen en OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

    // Configurar opciones de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ilDeleteImages(1, &imageID); // Liberar la imagen cargada

    return textureID;
}
