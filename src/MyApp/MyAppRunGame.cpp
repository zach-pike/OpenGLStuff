#include "MyApp.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"
#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"

#include "Camera/CameraController/CameraController.hpp"
#include "Camera/CameraController/CameraProgram/CameraProgram.hpp"
#include "CameraPrograms/QuarticBezierCurverProgram/QuarticBezierCurverProgram.hpp"

#include "Billboards/BillboardRender/BillboardRenderer.hpp"
#include "Billboards/TexturedBillboard/TexturedBillboard.hpp"

#include <iostream>
#include <exception>
#include <functional>

static std::unique_ptr<ShaderProgram> loadObjectShader() {
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.addSourceCodeFromPath("./shader/object/vertex.glsl");
    vertexShader.compile();

    if (!vertexShader.isShaderCompiled()) {
        std::string vertexShaderLog = vertexShader.getInfoLog();
        std::cout << vertexShaderLog << '\n';
        std::exit(1);
    }

    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.addSourceCodeFromPath("./shader/object/fragment.glsl");
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

void MyApp::init() {
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void MyApp::loop() {
    const char* vendor = (char*)glGetString(GL_VENDOR); // Returns the vendor
    const char* renderer = (char*)glGetString(GL_RENDERER); // Returns a hint to the model

    printf("Vendor: %s\nRenderer: %s\n", vendor, renderer);

    // VAO
    VertexArray vao;
    vao.bindVertexArray();

    // Simple buffer
    Buffer<glm::vec3> vertexBuffer;
    Buffer<glm::vec3> colorBuffer;

    glm::vec3 cubeVertexData[] = {
        glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3( 1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3( 1.0f,-1.0f,-1.0f),
		glm::vec3( 1.0f, 1.0f,-1.0f),
		glm::vec3( 1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3( 1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f,-1.0f, 1.0f),
		glm::vec3( 1.0f,-1.0f, 1.0f),
		glm::vec3( 1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f,-1.0f,-1.0f),
		glm::vec3( 1.0f, 1.0f,-1.0f),
		glm::vec3( 1.0f,-1.0f,-1.0f),
		glm::vec3( 1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f,-1.0f, 1.0f),
		glm::vec3( 1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3( 1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f,-1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f,-1.0f, 1.0f)
	};

    glm::vec3 cubeColorData[] = {
        glm::vec3(0.583f,  0.771f,  0.014f),
		glm::vec3(0.609f,  0.115f,  0.436f),
		glm::vec3(0.327f,  0.483f,  0.844f),
		glm::vec3(0.822f,  0.569f,  0.201f),
		glm::vec3(0.435f,  0.602f,  0.223f),
		glm::vec3(0.310f,  0.747f,  0.185f),
		glm::vec3(0.597f,  0.770f,  0.761f),
		glm::vec3(0.559f,  0.436f,  0.730f),
		glm::vec3(0.359f,  0.583f,  0.152f),
		glm::vec3(0.483f,  0.596f,  0.789f),
		glm::vec3(0.559f,  0.861f,  0.639f),
		glm::vec3(0.195f,  0.548f,  0.859f),
		glm::vec3(0.014f,  0.184f,  0.576f),
		glm::vec3(0.771f,  0.328f,  0.970f),
		glm::vec3(0.406f,  0.615f,  0.116f),
		glm::vec3(0.676f,  0.977f,  0.133f),
		glm::vec3(0.971f,  0.572f,  0.833f),
		glm::vec3(0.140f,  0.616f,  0.489f),
		glm::vec3(0.997f,  0.513f,  0.064f),
		glm::vec3(0.945f,  0.719f,  0.592f),
		glm::vec3(0.543f,  0.021f,  0.978f),
		glm::vec3(0.279f,  0.317f,  0.505f),
		glm::vec3(0.167f,  0.620f,  0.077f),
		glm::vec3(0.347f,  0.857f,  0.137f),
		glm::vec3(0.055f,  0.953f,  0.042f),
		glm::vec3(0.714f,  0.505f,  0.345f),
		glm::vec3(0.783f,  0.290f,  0.734f),
		glm::vec3(0.722f,  0.645f,  0.174f),
		glm::vec3(0.302f,  0.455f,  0.848f),
		glm::vec3(0.225f,  0.587f,  0.040f),
		glm::vec3(0.517f,  0.713f,  0.338f),
		glm::vec3(0.053f,  0.959f,  0.120f),
		glm::vec3(0.393f,  0.621f,  0.362f),
		glm::vec3(0.673f,  0.211f,  0.457f),
		glm::vec3(0.820f,  0.883f,  0.371f),
		glm::vec3(0.982f,  0.099f,  0.879f)
    };

    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec3> colorData;

    for (int x=0; x < 10; x++) {
        for (int z=0; z < 10; z++) {
            for (int y=0; y < 10; y++) {
                for (int j=0; j<36; j++) {
                    vertexData.push_back(cubeVertexData[j] + glm::vec3(4*x, 4*y, 4*z) + glm::vec3(10, 0, 10));
                    colorData.push_back(cubeColorData[j]);
                }
            }
        }
    }

    vertexBuffer.bufferData(vertexData);
    colorBuffer.bufferData(colorData);

    // Camera stuff
    CameraController cameraController(glm::vec3(4, 5, 0));

    std::vector<glm::vec3> cameraProgramControlPoints = {
        glm::vec3(20, .5, 5),
        glm::vec3(40, 3.5, 0),
        glm::vec3(30, 30, 25),
        glm::vec3(32, 40, 50)
    };
    auto bezierCameraProgram = std::make_shared<QuarticBezierCurverProgram>(cameraProgramControlPoints);
    // bezierCameraProgram->addSpeedFunction([](double x) { return .1 * std::cos(9.f * x) + .2 * std::sin(3.f * x - .6f) + .8; });

    BillboardRenderer billboardRenderer;

    auto texture = std::make_shared<Texture>();
    texture->loadFromFilePath("onebeerplease.jpg");

    for (int i=0; i<250; i++) {
        auto myTexturedBillboard = std::make_shared<TexturedBillboard>(billboardRenderer);
        myTexturedBillboard->billboardTexture = texture;
        myTexturedBillboard->size = glm::vec2(3, 7);
        myTexturedBillboard->position = glm::vec3(i*3, 0, 0);

        billboardRenderer.drawObjects.push_back(myTexturedBillboard);
    }

    // Object shader
    auto objectShader = loadObjectShader();
    GLuint objectViewProjectionUniform = objectShader->getUniformLocation("viewProjection");

    glClearColor(.25f, .5f, .75f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // Key press variables
    bool isProgramButtonPressed = false;
    bool isPathTogglePressed    = false;

    bool isShowingPath = false;

    double lastFrameStartTime = glfwGetTime();
    
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
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        glm::mat4 viewProjection = projectionMatrix * viewMatrix;
        
        // Camera controller logic
        cameraController.step(window, deltaTime);

        // Start camera program
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (isProgramButtonPressed == false) {
                isProgramButtonPressed = true;

                bezierCameraProgram->reset();
                cameraController.useProgram(bezierCameraProgram);
            }
        } else {
            isProgramButtonPressed = false;
        }

        // Toggle camera path
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            if (isPathTogglePressed == false) {
                isPathTogglePressed = true;

                isShowingPath = !isShowingPath;
            }
        } else {
            isPathTogglePressed = false;
        }

        // Drawing code
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Controls");
            ImGui::InputFloat("x1", &(cameraProgramControlPoints[0].x));
            ImGui::InputFloat("y1", &(cameraProgramControlPoints[0].y));
            ImGui::InputFloat("z1", &(cameraProgramControlPoints[0].z));

            ImGui::InputFloat("x2", &(cameraProgramControlPoints[1].x));
            ImGui::InputFloat("y2", &(cameraProgramControlPoints[1].y));
            ImGui::InputFloat("z2", &(cameraProgramControlPoints[1].z));

            ImGui::InputFloat("x3", &(cameraProgramControlPoints[2].x));
            ImGui::InputFloat("y3", &(cameraProgramControlPoints[2].y));
            ImGui::InputFloat("z3", &(cameraProgramControlPoints[2].z));

            ImGui::InputFloat("x4", &(cameraProgramControlPoints[3].x));
            ImGui::InputFloat("y4", &(cameraProgramControlPoints[3].y));
            ImGui::InputFloat("z4", &(cameraProgramControlPoints[3].z));

            if (ImGui::Button("Update")) {
                bezierCameraProgram->setControlPoints(cameraProgramControlPoints);
            }

        ImGui::End();

        billboardRenderer.draw(viewMatrix, viewProjection);

        if (isShowingPath) bezierCameraProgram->drawPath(viewMatrix, viewProjection);

        objectShader->use();
        glUniformMatrix4fv(objectViewProjectionUniform, 1, GL_FALSE, &viewProjection[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getBufferId());
        glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer.getBufferId());
        glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Render objects
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void MyApp::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void MyApp::runGame() {
    init();
    loop();
    cleanup();
}