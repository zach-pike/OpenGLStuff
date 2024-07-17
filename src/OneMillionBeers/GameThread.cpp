#include "OneMillionBeers.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"
#include "Utility/GL/Texture/Texture.hpp"

#include "Camera/CameraController/CameraController.hpp"

#include <iostream>
#include <vector>

static std::unique_ptr<ShaderProgram> loadShader() {
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.addSourceCodeFromPath("./shader/instancing/vertex.glsl");
    vertexShader.compile();

    if (!vertexShader.isShaderCompiled()) {
        std::string vertexShaderLog = vertexShader.getInfoLog();
        std::cout << vertexShaderLog << '\n';
        std::exit(1);
    }

    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.addSourceCodeFromPath("./shader/instancing/fragment.glsl");
    fragmentShader.compile();

    if (!fragmentShader.isShaderCompiled()) {
        std::string fragmentShaderLog = fragmentShader.getInfoLog();
        std::cout << fragmentShaderLog << '\n';
        std::exit(1);
    }

    auto program = std::make_unique<ShaderProgram>();

    program->addShader(std::move(vertexShader));
    program->addShader(std::move(fragmentShader));

    program->linkProgram();

    if (!program->isProgramLinked()) {
        std::string programLog = program->getInfoLog();
        std::cout << programLog << '\n';
        std::exit(1);
    }

    program->getShaderList().clear();

    return program;
}


void OneMillionBeers::init() {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW!\n";
        std::exit(1);
    }

    window = glfwCreateWindow(1024, 768, "Window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load OpenGL/GLAD!\n";
        std::exit(1);
    }

    glClearColor(.25f, .5f, .75f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    const char* vendor = (char*)glGetString(GL_VENDOR); // Returns the vendor
    const char* renderer = (char*)glGetString(GL_RENDERER); // Returns a hint to the model

    printf("Vendor: %s\nRenderer: %s\n", vendor, renderer);
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

void OneMillionBeers::loop() {
    // VAO
    VertexArray vao;
    vao.bindVertexArray();

    Buffer<glm::vec2> billboardVertexBuffer;
    Buffer<glm::vec2> billboardUVBuffer;
    Buffer<glm::vec3> billboardPositionBuffer;

    billboardVertexBuffer.bufferData(billboardUVData);
    billboardUVBuffer.bufferData(billboardUVData);

    CameraController cameraController(glm::vec3(0, 0, 0));

    auto shaderProgram = loadShader();
    GLuint textureUniform = shaderProgram->getUniformLocation("myTexture");

    GLuint viewMatrixUniform = shaderProgram->getUniformLocation("viewMatrix");
    GLuint viewProjectionUniform = shaderProgram->getUniformLocation("viewProjection");

    GLuint cameraPositionUniform = shaderProgram->getUniformLocation("cameraPosition");

    GLuint fogMinUniform = shaderProgram->getUniformLocation("fogMin");
    GLuint fogMaxUniform = shaderProgram->getUniformLocation("fogMax");
    GLuint fogColorUniform = shaderProgram->getUniformLocation("fogColor");

    Texture beerTexture;
    beerTexture.loadFromFilePath("onebeerplease.jpg");

    const int BillboardCount = 1000000;

    std::vector<glm::vec3> posArray;

    for (int i=0; i<BillboardCount; i++) {
        float x = i % 500;
        float z = i / 500;

        posArray.push_back(glm::vec3(x, 0.f, z)*glm::vec3(5));
    }

    billboardPositionBuffer.bufferData(posArray);

    shaderProgram->use();

    // Set fog parameters
    const float fogMinimim = 500.f;
    const float fogMaximum = 1000.f;
    const glm::vec3 fogColor(.25f, .5f, .75f);

    glUniform1f(fogMinUniform, fogMinimim);
    glUniform1f(fogMaxUniform, fogMaximum);
    glUniform3fv(fogColorUniform, 1, &fogColor[0]);

    // Use texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, beerTexture.getTextureId());
    glUniform1i(textureUniform, 0);

    double lastFrameStartTime = glfwGetTime();

    // Setup our vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer.getBufferId());
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, billboardUVBuffer.getBufferId());
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, billboardPositionBuffer.getBufferId());
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Only change billboard position when we start a new instance
    glVertexAttribDivisor(2, 1);

    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        double frameStartTime = glfwGetTime();

        // DeltaT calc
        double deltaTime = frameStartTime - lastFrameStartTime;
    
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        
        // 3D Math stuff...
        glm::mat4 viewMatrix = cameraController.getViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, fogMaximum);

        glm::mat4 viewProjection = projectionMatrix * viewMatrix;
        
        // Camera controller logic
        cameraController.step(window, deltaTime);
        glm::vec3 cameraPosition = cameraController.getCamera().pos;

        // Send the matrices to the gpu
        glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, &viewProjection[0][0]);

        // Used for fog calculation
        glUniform3fv(cameraPositionUniform, 1, &cameraPosition[0]);

        // Render objects
        glDrawArraysInstanced(GL_QUADS, 0, 4, BillboardCount);

        // Frame limiter
        const double frameEndTime = glfwGetTime();
        const double frameTimeMS = (frameEndTime - frameStartTime) * 1e6;
        const double requiredFrameTimeMS = 8333.33;
        const double sleepTime = requiredFrameTimeMS - frameTimeMS;

        if (sleepTime > 0) std::this_thread::sleep_for(std::chrono::microseconds((std::int64_t)sleepTime));

        // Update dt calculation
        lastFrameStartTime = frameStartTime;

        glfwSwapBuffers(window);
        
        // Make viewport always the same size as window
        glViewport(0, 0, windowWidth, windowHeight);
    }
}

void OneMillionBeers::cleanup() {
    glfwTerminate();
}

void OneMillionBeers::_gameThread() {
    init();
    loop();
    cleanup();
}