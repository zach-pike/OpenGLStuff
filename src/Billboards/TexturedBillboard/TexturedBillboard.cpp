#include "TexturedBillboard.hpp"

#include <iostream>

std::shared_ptr<ShaderProgram> TexturedBillboard::loadShader() {
    // Vertex shader
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.addSourceCodeFromPath("./shader/billboard/textured/vertex.glsl");
    vertexShader.compile();

    if (!vertexShader.isShaderCompiled()) {
        std::string vertexShaderLog = vertexShader.getInfoLog();
        std::cout << vertexShaderLog << '\n';
        std::exit(1);
    }

    // Fragment shader
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.addSourceCodeFromPath("./shader/billboard/textured/fragment.glsl");
    fragmentShader.compile();

    if (!fragmentShader.isShaderCompiled()) {
        std::string fragmentShaderLog = fragmentShader.getInfoLog();
        std::cout << fragmentShaderLog << '\n';
        std::exit(1);
    }

    auto program = std::make_shared<ShaderProgram>();

    program->addShader(std::move(vertexShader));
    program->addShader(std::move(fragmentShader));

    program->linkProgram();
    if (!program->isProgramLinked()) {
        std::string programLog = program->getInfoLog();
        std::cout << programLog << '\n';
        std::exit(1);
    }

    program->getShaderList().clear(); // Clear shaders not needed anymore after linking program

    return program;
}

static std::vector<glm::vec2> billboardVertexData = {
    glm::vec2(-0.5f, -0.5f),
    glm::vec2(-0.5f, 0.5f),
    glm::vec2(0.5f, 0.5f),
    glm::vec2(0.5f, -0.5f),
};

static std::vector<glm::vec2> billboardUVData = {
    glm::vec2(0, 1),
    glm::vec2(0, 0),
    glm::vec2(1, 0),
    glm::vec2(1, 1),
};

TexturedBillboard::TexturedBillboard(BillboardRenderer& renderer) {
    // Check if renderer already has a copy of the shader program
    if (renderer.loadedShaders.count("TexturedBillboard") == 0) {
        auto loadedShader = loadShader();

        renderer.loadedShaders.insert({ "TexturedBillboard", loadedShader });
        shader = loadedShader;
    } else {
        shader = renderer.loadedShaders.at("TexturedBillboard");
    }


    // Shader uniforms
    billboardSizeUniform           = shader->getUniformLocation("billboardSize");
    billboardPositionUniform       = shader->getUniformLocation("billboardPosition");

    billboardViewProjectionUniform = shader->getUniformLocation("viewProjection");
    billboardViewMatrixUniform     = shader->getUniformLocation("viewMatrix");

    billboardTextureUniform        = shader->getUniformLocation("myTexture");

    // Billboard Data
    billboardVertexBuffer.bufferData(billboardVertexData);
    billboardUVBuffer.bufferData(billboardUVData);
}

TexturedBillboard::~TexturedBillboard() {}

void TexturedBillboard::draw(glm::mat4 viewMatrix, glm::mat4 viewProjection) {
    shader->use();

    glUniformMatrix4fv(billboardViewMatrixUniform, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(billboardViewProjectionUniform, 1, GL_FALSE, &viewProjection[0][0]);

    // Start using texture unit 0
    glActiveTexture(GL_TEXTURE0); 

    // Bind billboardTexture to texture unit 0
    glBindTexture(GL_TEXTURE_2D, billboardTexture->getTextureId());
    glUniform1i(billboardTextureUniform, 0);

    // Set position
    glUniform3fv(billboardPositionUniform, 1, &position[0]);
    glUniform2fv(billboardSizeUniform, 1, &size[0]);

    glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer.getBufferId());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, billboardUVBuffer.getBufferId());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);    
}