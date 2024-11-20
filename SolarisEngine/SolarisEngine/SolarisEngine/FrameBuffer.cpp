#include "FrameBuffer.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <GL/glew.h> // Incluye GLEW antes de OpenGL
#include <GL/gl.h>
#include <iostream>
#include <exception>

FrameBuffer::FrameBuffer(float width, float height)
    : fbo(0), texture(0), rbo(0)
{
    // Inicialización de GLEW (realiza esto en la inicialización principal si es posible)
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW." << std::endl;
        throw std::runtime_error("GLEW initialization failed");
    }

    // Genera el framebuffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Genera la textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
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

void FrameBuffer::RescaleFrameBuffer() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Cambiar GL_RGB a GL_RGBA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void FrameBuffer::SetScale(float width, float height)
{
    this->width = width;
    this->height = height;
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::Unbind() const {
    GLint currentFbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFbo);
    if (currentFbo == fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
