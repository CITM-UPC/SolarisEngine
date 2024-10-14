#include <GL/glew.h>
#include <chrono>
#include <thread>
#include <exception>
#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>
#include "MyWindow.h"

#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <iostream>




using namespace std;

using hrclock = chrono::high_resolution_clock;
using u8vec4 = glm::u8vec4;
using ivec2 = glm::ivec2;
using vec3 = glm::dvec3;

static const ivec2 WINDOW_SIZE(512, 512);
static const unsigned int FPS = 60;
static const auto FRAME_DT = 1.0s / FPS;


const char* file = "h.fbx";
const struct aiScene* scene = aiImportFile(file, aiProcess_Triangulate);

static const int CHECKERS_HEIGHT = 64;
static const int CHECKERS_WIDTH = 64;



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

std::vector<GLuint> VAOs(scene->mNumMeshes);
std::vector<GLuint> VBOs(scene->mNumMeshes);

static void DrawFBX() {
	if (!scene) {
		fprintf(stderr, "Error al cargar el archivo: %s\n", aiGetErrorString());
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

	
		glGenVertexArrays(1, &VAOs[i]);
		glGenBuffers(1, &VBOs[i]);

		glBindVertexArray(VAOs[i]);

		float multi = 1;

		// Cargar los vértices
		std::vector<GLfloat> vertices(mesh->mNumVertices * 3);
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			aiVector3D vertex = mesh->mVertices[v];
			vertices[v * 3] = vertex.x * multi;
			vertices[v * 3 + 1] = vertex.y * multi;
			vertices[v * 3 + 2] = vertex.z * multi;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);



		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// Dibuja la malla
		glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices);

		
		// Limpiar
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);



		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			glBindVertexArray(VAOs[i]);
			glDrawArrays(GL_TRIANGLES, 0, scene->mMeshes[i]->mNumVertices);
		}

		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			glDeleteVertexArrays(1, &VAOs[i]);
			glDeleteBuffers(1, &VBOs[i]);
		}

		
	}
}

GLuint textureID;

void initializeDevIL() {
	ilInit();
	iluInit();
}

bool loadImageWithDevIL(const char* filename) {
	ILuint imageID;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (!ilLoadImage((const wchar_t*)filename)) {
		std::cerr << "Error loading image: " << ilGetError() << std::endl;
		ilDeleteImages(1, &imageID);
		return false;
	}

	// Obtener el tamaño de la imagen
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	// Obtener los datos de los píxeles
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	GLubyte* imageData = ilGetData();

	// Generar la textura en OpenGL
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// Limpiar
	ilDeleteImages(1, &imageID);
	return true;
}

static void drawTexture(const char* imagePath) {
	initializeDevIL();

	// Cargar la imagen como textura
	if (!loadImageWithDevIL(imagePath)) {
		return;
	}

	// Asegúrate de que tu escena sea válida
	if (!scene) {
		//std::cerr << "Error al cargar el archivo: " << aiGetErrorString() << std::endl;
		return;
	}

	// Habilitar texturas
	glEnable(GL_TEXTURE_2D);

	// Iterar sobre los meshes de la escena
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		// Crear VAO y VBO
		GLuint VAO, VBO, texCoordVBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &texCoordVBO);

		glBindVertexArray(VAO);


		float multi = 0.05f;

		// Cargar los vértices
		std::vector<GLfloat> vertices(mesh->mNumVertices * 3);
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			aiVector3D vertex = mesh->mVertices[v];
			vertices[v * 3] = vertex.x * multi;
			vertices[v * 3 + 1] = vertex.y * multi;
			vertices[v * 3 + 2] = vertex.z * multi;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		// Cargar coordenadas de textura
		std::vector<GLfloat> texCoords(mesh->mNumVertices * 2);
		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			if (mesh->HasTextureCoords(0)) {
				aiVector3D texCoord = mesh->mTextureCoords[0][v];
				texCoords[v * 2] = texCoord.x;  // Usar las coordenadas directamente
				texCoords[v * 2 + 1] = texCoord.y;
			}
			else {
				texCoords[v * 2] = 0.0f; // Valor por defecto si no hay coordenadas de textura
				texCoords[v * 2 + 1] = 0.0f;
			}
		}

		// Cargar el VBO de coordenadas de textura
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);

		// Dibuja la malla
		glBindTexture(GL_TEXTURE_2D, textureID); // Asegúrate de que la textura esté activada
		glDrawArrays(GL_TRIANGLES, 0, mesh->mNumVertices);

		// Limpiar
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Eliminar VAOs y VBOs después de usar
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &texCoordVBO);
	}

	// Deshabilitar texturas
	glDisable(GL_TEXTURE_2D);
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
	//draw_triangle(u8vec4(255, 0, 0, 255), vec3(0.5, 0.0, 0.0), 0.5);
	//draw_cube();


	/*DrawFBX();
	glTexCoordPointer();*/
	drawTexture("cat.jpg");
	glRotatef(1.0f, 1.0f, 1.0f, 1.0f);


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

