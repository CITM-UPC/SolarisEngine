#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"





using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;

static void init_openGL() {
	glewInit();
	if (!GLEW_VERSION_3_0) throw exception("OpenGL 3.0 API is not  available.");
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

static void draw_cube() {
	GLfloat vertices[] = {
		// Cara frontal
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		// Cara trasera
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,

		 // Cara izquierda
		 -0.5f, -0.5f, -0.5f,
		 -0.5f, -0.5f,  0.5f,
		 -0.5f,  0.5f,  0.5f,
		 -0.5f, -0.5f, -0.5f,
		 -0.5f,  0.5f,  0.5f,
		 -0.5f,  0.5f, -0.5f,

		 // Cara derecha
		  0.5f, -0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,
		  0.5f, -0.5f,  0.5f,
		  0.5f, -0.5f, -0.5f,
		  0.5f,  0.5f, -0.5f,
		  0.5f,  0.5f,  0.5f,

		  // Cara superior
		  -0.5f,  0.5f, -0.5f,
		  -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		  -0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,

		   // Cara inferior
		   -0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f, -0.5f,
		   -0.5f, -0.5f, -0.5f,
		   -0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
	};

	// activar y especificar el puntero al arreglo de vértices
	glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    // Dibujar cada cara con un color diferente
    // Cara frontal (rojo)
    glColor3f(1.0f, 0.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Cara trasera (verde)
    glColor3f(0.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 6, 6);

    // Cara izquierda (azul)
    glColor3f(0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 12, 6);

    // Cara derecha (amarillo)
    glColor3f(1.0f, 1.0f, 0.0f);
    glDrawArrays(GL_TRIANGLES, 18, 6);

    // Cara superior (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 24, 6);

    // Cara inferior (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 30, 6);

    // Desactivar arreglos de vértices después de dibujar
    glDisableClientState(GL_VERTEX_ARRAY);
	
	// Dibujar las aristas
	glColor3f(1.0f, 0.0f, 0.0f); // Color rojo para las aristas
	glLineWidth(2.0f);
	GLfloat edges[] = {
		// Aristas
		-0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
	};

	// Activar y especificar el puntero al arreglo de vértices para las aristas
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, edges);

	// Dibujar las aristas
	glDrawArrays(GL_LINES, 0, 24); // 24 aristas

	// Desactivar arreglos de vértices después de dibujar
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnd();

}

static void draw_triangle(const u8vec4& color, const vec3& center, double size) {
	/*glColor4ub(color.r, color.g, color.b, color.a);
	glBegin(GL_TRIANGLES);
	glVertex3d(center.x, center.y + size, center.z);
	glVertex3d(center.x - size, center.y - size, center.z);
	glVertex3d(center.x + size, center.y - size, center.z);
	glEnd();*/


	/*glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, -0.3f, 0.f);
	glVertex3f(0.f, 0.3f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(-0.3f, 0.f, 0.f);
	glVertex3f(0.3f, 0.f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.3f, 0.f, 0.f);
	glVertex3f(0.3f, -0.3f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(-0.3f, -0.0f, 0.f);
	glVertex3f(-0.3f, 0.3f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, -0.3f, 0.f);
	glVertex3f(-0.3f, -0.3f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.f, 0.3f, 0.f);
	glVertex3f(0.3f, 0.3f, 00.f);
	glEnd();

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(0.3f, 0.3f, 0.f);
	glVertex3f(0.3f, 0.f, 00.f);
	glEnd();*/

	
	
}

static void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.0, 0.0, 0.0), 0.5);
	draw_cube();
	glRotatef(0.1f, 1.0f, 1.0f, 0.0f);



}
#include "imgui_impl_sdl2.h"

static bool processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
			break;
		default:
			ImGui_ImplSDL2_ProcessEvent(&event);
			break;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	MyWindow window("SDL2 Simple Example", WINDOW_SIZE.x, WINDOW_SIZE.y);

	init_openGL();

	while (processEvents()) {
		const auto t0 = hrclock::now();
		display_func();
		window.swapBuffers();
		const auto t1 = hrclock::now();
		const auto dt = t1 - t0;
		if(dt<FRAME_DT) this_thread::sleep_for(FRAME_DT - dt);
	}

	return 0;
}