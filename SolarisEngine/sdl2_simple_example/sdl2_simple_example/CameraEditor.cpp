#include "CameraEditor.h"
#include <SDL2/SDL_scancode.h>


glm::mat4x4 CameraEditor::lookAt(vec3  const& eye, vec3  const& center, vec3  const& up)
{
    vec3  f = normalize(center - eye);
    vec3  u = normalize(up);
    vec3  s = normalize(cross(f, u));
    u = cross(s, f);

    glm::mat4x4 Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = -f.x;
    Result[1][2] = -f.y;
    Result[2][2] = -f.z;
    Result[3][0] = -dot(s, eye);
    Result[3][1] = -dot(u, eye);
    Result[3][2] = dot(f, eye);
    return Result;
}

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

float fov = glm::radians(90.0f); // Campo de visión en radianes
float nearPlane = 0.01f; // Plano cercano
float farPlane = 100.0f; // Plano lejano
float aspectRatio = (float)512  / (float)512; // Relación de aspecto

glm::mat4 projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);


// Constructor de la cámara
CameraEditor::CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up)
    : position(position), front(front), up(up) {}

// Devuelve la matriz de vista de la cámara
glm::mat4 CameraEditor::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

// Procesa las entradas del teclado para mover la cámara
void CameraEditor::processInput(unsigned char key) {
    const float cameraSpeed = 0.05f; // Velocidad de movimiento de la cámara
    if (key == 'w') {
        position += cameraSpeed * front; // Mover hacia adelante
    }
    if (key == 's') {
        position -= cameraSpeed * front; // Mover hacia atrás
    }
    if (key == 'a') {
        position -= glm::normalize(glm::cross(front, up)) * cameraSpeed; // Mover a la izquierda
    }
    if (key == 'd') {
        position += glm::normalize(glm::cross(front, up)) * cameraSpeed; // Mover a la derecha
    }
    if (key == 'q') {
        position += cameraSpeed * up; // Mover hacia arriba
    }
    if (key == SDL_SCANCODE_LCTRL) {
        position -= cameraSpeed * up; // Mover hacia abajo
    }
}

void CameraEditor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; // Ajustar rotación alrededor del eje Y
    pitch -= yoffset; // Ajustar rotación alrededor del eje X

    // Limitar el pitch para evitar problemas de "gimbal lock"
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calcular la nueva dirección de la cámara
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
}

void CameraEditor::updatePosition(glm::vec3 delta) {
    position += delta; // Método para actualizar la posición
}


void CameraEditor::Update() {

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection)); // Cargar la matriz de proyección

    glm::mat4 view = getViewMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view)); // Cargar la matriz de vista
}
