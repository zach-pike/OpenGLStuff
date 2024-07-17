#pragma once

#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"
#include "BillboardObject/BillboardObject.hpp"

#include <string>
#include <map>
#include <memory>

class BillboardRenderer {
public:
    // Keep track of loaded shaders for billboards
    std::map<std::string, std::shared_ptr<ShaderProgram>> loadedShaders;

    // Draw list
    std::vector<std::shared_ptr<BillboardObject>> drawObjects;

    BillboardRenderer();
    ~BillboardRenderer();

    void draw(glm::mat4 viewMatrix, glm::mat4 viewProjection);
};