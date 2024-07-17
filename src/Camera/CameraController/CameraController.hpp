#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera/Camera.hpp"
#include "CameraProgram/CameraProgram.hpp"
#include <memory>

class CameraController {
private:
    double lastMouseX, lastMouseY;
    Camera camera;

    bool isMouseLockedButtonPressed = false;
    bool isMouseLocked = true;

    bool isFirstStep = true;

    std::shared_ptr<CameraProgram> cameraProgram = nullptr;
    CameraProgram::CameraData      cameraProgramData = {  };

    void lockMouse(GLFWwindow* window);
    void unlockMouse(GLFWwindow* window);
public:
    CameraController(glm::vec3 cameraPos);
    ~CameraController();

    glm::mat4 getViewMatrix() const;
    const Camera& getCamera() const;

    void useProgram(std::shared_ptr<CameraProgram> program);

    void step(GLFWwindow* window, double deltaTime);
};