// Ray.h
#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& orig, const glm::vec3& dir)
        : origin(orig), direction(glm::normalize(dir)) {}
};

#endif // RAY_H
