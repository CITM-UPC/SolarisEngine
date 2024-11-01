#include "FrameBuffer.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <GL/gl.h> // Incluye OpenGL sin GLEW
#include <iostream>
#include <exception>

FrameBuffer::FrameBuffer(float width, float height)
    : fbo(0), texture(0), rbo(0) // Inicializa a 0 para evitar uso de variables no inicializadas
{
    //// Asegúrate de que el contexto de OpenGL es válido
    //if (!app->windowEditor || !app->windowEditor->_window || !app->windowEditor->_ctx) {
    //    std::cerr << "ERROR::FRAMEBUFFER:: Invalid window or context!" << std::endl;
    //    return;
    //}

    //// Haz que el contexto de OpenGL sea actual
    //if (SDL_GL_MakeCurrent(app->windowEditor->_window, app->windowEditor->_ctx) != 0) {
    //    std::cerr << "ERROR::FRAMEBUFFER:: SDL_GL_MakeCurrent failed! SDL_Error: " << SDL_GetError() << std::endl;
    //    return;
    //}

    // Genera el framebuffer
    glGenFramebuffers(1, &fbo);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "ERROR::FRAMEBUFFER:: glGenFramebuffers failed! Error code: " << error << std::endl;
        return; // Maneja el error
    }

    std::cout << "Framebuffer ID: " << fbo << std::endl; // Debería ser válido ahora

    // Vincula el framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "ERROR::FRAMEBUFFER:: glBindFramebuffer failed! Error code: " << error << std::endl;
        return; // Maneja el error
    }

    // Genera la textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Genera el renderbuffer
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Comprobación del estado del framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }

    // Desvincula el framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &rbo);
}

unsigned int FrameBuffer::getFrameTexture()
{
    return texture;
}

void FrameBuffer::RescaleFrameBuffer(float width, float height)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
