#include "QuarticBezierCurverProgram.hpp"

glm::vec3 QuarticBezierCurverProgram::calculateBezier(double _t) {
    double t1 = (1.0 - _t);
     double t2 = _t * _t;

    double b3 = t2 * _t;
    double b2 = 3.0 * t2 * t1;
    double b1 = 3.0 * _t * t1 * t1;
    double b0 = t1 * t1 * t1;

     return controlPoints[0] * glm::vec3(b0) 
        + controlPoints[1] * glm::vec3(b1) 
        + controlPoints[2] * glm::vec3(b2) 
        + controlPoints[3] * glm::vec3(b3);
}

std::unique_ptr<ShaderProgram> QuarticBezierCurverProgram::loadBezierCurveShader() {
    // Vertex shader
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.addSourceCodeFromPath("shader/bezier/vertex.glsl");
    vertexShader.compile();

    if (!vertexShader.isShaderCompiled()) {
        std::string vertexShaderLog = vertexShader.getInfoLog();
        std::cout << vertexShaderLog << '\n';
        std::exit(1);
    }

    // Tesselation control shader
    Shader tcsShader(GL_TESS_CONTROL_SHADER);
    tcsShader.addSourceCodeFromPath("shader/bezier/tcs.glsl");
    tcsShader.compile();

    if (!tcsShader.isShaderCompiled()) {
        std::string tcsShaderLog = tcsShader.getInfoLog();
        std::cout << tcsShaderLog << '\n';
        std::exit(1);
    }

    // Tesselation evaluation shader
    Shader tesShader(GL_TESS_EVALUATION_SHADER);
    tesShader.addSourceCodeFromPath("shader/bezier/tes.glsl");
    tesShader.compile();

    if (!tesShader.isShaderCompiled()) {
        std::string tesShaderLog = tesShader.getInfoLog();
        std::cout << tesShaderLog << '\n';
        std::exit(1);
    }

    // Fragment shader
    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.addSourceCodeFromPath("shader/bezier/fragment.glsl");
    fragmentShader.compile();

    if (!fragmentShader.isShaderCompiled()) {
        std::string fragmentShaderLog = fragmentShader.getInfoLog();
        std::cout << fragmentShaderLog << '\n';
        std::exit(1);
    }

    auto program = std::make_unique<ShaderProgram>();

    program->addShader(std::move(vertexShader));
    program->addShader(std::move(tcsShader));
    program->addShader(std::move(tesShader));
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

QuarticBezierCurverProgram::QuarticBezierCurverProgram(std::vector<glm::vec3> _controlPoints) {
    controlPoints = _controlPoints;
    bezierVertexBuffer.bufferData(controlPoints);

    bezierShader = loadBezierCurveShader();
    bezierCurveViewProjectionUniform = bezierShader->getUniformLocation("viewProjection");

}

QuarticBezierCurverProgram::~QuarticBezierCurverProgram() {}

CameraProgram::CameraData QuarticBezierCurverProgram::tick(double dt) {
    CameraProgram::CameraData data = { };

    data.position = calculateBezier(t);
    data.looking  = calculateBezier(t + speed * dt * speedFunc(t)) - calculateBezier(t);
    data.up       = glm::vec3(-std::sqrt(2.f)/2, std::sqrt(2.f)/2, 0);
    data.done = t >= 1.f;

    t += speed * dt * speedFunc(t);

    return data;
}

void QuarticBezierCurverProgram::reset() {
    t = 0.f;
}

void QuarticBezierCurverProgram::drawPath(glm::mat4 viewMatrix, glm::mat4 viewProjection) {
    // For drawing bezier curves
    glPatchParameteri(GL_PATCH_VERTICES, 4);

    bezierShader->use();
    glUniformMatrix4fv(bezierCurveViewProjectionUniform, 1, GL_FALSE, &viewProjection[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, bezierVertexBuffer.getBufferId());
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_PATCHES, 0, 4);

    glDisableVertexAttribArray(0);
}

void QuarticBezierCurverProgram::addSpeedFunction(std::function<double(double)> someFunction) {
    speedFunc = someFunction;
}

void QuarticBezierCurverProgram::setControlPoints(std::vector<glm::vec3> points) {
    controlPoints = points;
    bezierVertexBuffer.bufferData(controlPoints);
}