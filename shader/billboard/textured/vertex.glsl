#version 410 core

layout(location=0) in vec2 vertexPosition;
layout(location=1) in vec2 inUV;

uniform vec2 billboardSize;
uniform vec3 billboardPosition;

uniform mat4 viewMatrix;
uniform mat4 viewProjection;

out vec2 outUV;
 
void main() {
    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    vec3 vertexPositionWorldspace =
        billboardPosition
        + cameraRight * vertexPosition.x * billboardSize.x
        + cameraUp * vertexPosition.y * billboardSize.y;
    
    gl_Position = viewProjection * vec4(vertexPositionWorldspace, 1);

    outUV = inUV;
}