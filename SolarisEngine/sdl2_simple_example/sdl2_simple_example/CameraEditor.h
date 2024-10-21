#ifndef CAMERA_EDITOR_H
#define CAMERA_EDITOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraEditor {
public:
    CameraEditor(glm::vec3 position, glm::vec3 front, glm::vec3 up);

    glm::mat4 getViewMatrix() const;
    void processInput(unsigned char key);
    void processMouseMovement(float xoffset, float yoffset);
    void updatePosition(glm::vec3 delta);

private:
    glm::vec3 position; // Posición de la cámara
    glm::vec3 front;    // Dirección hacia donde apunta la cámara
    glm::vec3 up;       // Vector hacia arriba de la cámara

    float yaw;          // Rotación alrededor del eje Y
    float pitch;        // Rotación alrededor del eje X
    const float sensitivity = 0.1f; // Sensibilidad del movimiento del ratón
};

#endif // CAMERA_EDITOR_H
