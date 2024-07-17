#pragma once

#include <glm/glm.hpp>


class BillboardObject {
public:
    virtual void draw(glm::mat4 viewMatrix, glm::mat4 viewProjection) = 0;  
};