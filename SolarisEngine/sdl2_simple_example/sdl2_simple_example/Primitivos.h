#pragma once
#include <GL/glew.h>
#include "Cube.h"


struct CubeImmediateMode : public Cube
{

	void draw();

	CubeImmediateMode();
};