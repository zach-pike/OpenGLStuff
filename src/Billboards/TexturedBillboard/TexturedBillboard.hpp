#pragma once

#include "Billboards/BillboardRender/BillboardObject/BillboardObject.hpp"
#include "Billboards/BillboardRender/BillboardRenderer.hpp"

#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/Texture/Texture.hpp"

#include <memory>

class TexturedBillboard : public BillboardObject {
private:
    Buffer<glm::vec2> billboardVertexBuffer;
    Buffer<glm::vec2> billboardUVBuffer;

    std::shared_ptr<ShaderProgram> shader;
    GLuint billboardSizeUniform;
    GLuint billboardPositionUniform;

    GLuint billboardViewProjectionUniform;
    GLuint billboardViewMatrixUniform;

    GLuint billboardTextureUniform;

    static std::shared_ptr<ShaderProgram> loadShader();
public:
    glm::vec3 position;
    glm::vec2 size;
    std::shared_ptr<Texture>           billboardTexture;

    TexturedBillboard(BillboardRenderer& renderer);
    ~TexturedBillboard();

    void draw(glm::mat4 viewMatrix, glm::mat4 viewProjection);
};