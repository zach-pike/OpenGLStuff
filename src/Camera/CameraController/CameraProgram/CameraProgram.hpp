#pragma once

#include <glm/glm.hpp>

class CameraProgram {
public:
    struct CameraData {
        glm::vec3 position;
        glm::vec3 looking;
        glm::vec3 up;
        bool done;
    };

    virtual CameraData tick(double deltaTime) = 0;
    virtual void reset() = 0;
    virtual void drawPath(glm::mat4 viewMatrix, glm::mat4 viewProjection) = 0;
};