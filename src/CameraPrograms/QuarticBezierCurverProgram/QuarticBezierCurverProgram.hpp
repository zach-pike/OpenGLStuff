#pragma once

#include <memory>
#include <functional>
#include <iostream>

#include "Camera/CameraController/CameraProgram/CameraProgram.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"

#include <glm/glm.hpp>

class QuarticBezierCurverProgram : public CameraProgram {
private:
    std::vector<glm::vec3> controlPoints;
    Buffer<glm::vec3> bezierVertexBuffer;

    double t = 0.f;
    const double speed = .1f;

    std::unique_ptr<ShaderProgram> bezierShader;
    GLuint bezierCurveViewProjectionUniform;

    std::function<double(double)> speedFunc = [](double x) { return 1.0f; };

    glm::vec3 calculateBezier(double _t);

    std::unique_ptr<ShaderProgram> loadBezierCurveShader();
public:
    QuarticBezierCurverProgram(std::vector<glm::vec3> controlPoints);
    ~QuarticBezierCurverProgram();

    CameraProgram::CameraData tick(double dt);
    void reset();
    void drawPath(glm::mat4 viewMatrix, glm::mat4 viewProjection);
    void addSpeedFunction(std::function<double(double)> someFunction);
    void setControlPoints(std::vector<glm::vec3> points);
};