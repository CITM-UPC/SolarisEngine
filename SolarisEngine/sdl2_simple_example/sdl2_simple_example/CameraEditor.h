// CameraEditor.h
#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include <glm/glm.hpp>
#include <GL/glew.h> // Incluir GLEW antes de OpenGL
#include <GL/gl.h> // Cambiar a OpenGL (quitar GLUT)
#include <glm/gtc/type_ptr.hpp> // Para glm::value_ptr


class App;  // Declaración anticipada para que el compilador conozca la clase App.

class CameraEditor {
public:
    CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    glm::mat4 getViewMatrix() const;
    void processInput(unsigned char key);
    void processMouseMovement(float xoffset, float yoffset);
    void updatePosition(glm::vec3 delta);
    void Update();
    void MouseWheel(bool zoom);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw = 0.0f; // Ángulo de rotación alrededor del eje Y
    float pitch = 0.0f; // Ángulo de rotación alrededor del eje X
    float sensitivity = 0.1f; // Sensibilidad del ratón
};

#endif // CAMERA_EDITOR_H
