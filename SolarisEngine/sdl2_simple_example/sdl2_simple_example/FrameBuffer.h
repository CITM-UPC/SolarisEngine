// FrameBuffer.h

#pragma once
#include <string>
#include "App.h"
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include "WindowImGui.h" // Incluir la clase WindowImGui
#include "Defs.h" // Incluir la clase WindowImGui


class FrameBuffer
{
public:
	FrameBuffer(float width, float height);
	~FrameBuffer();
	unsigned int getFrameTexture();
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;

	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;

private:

};
