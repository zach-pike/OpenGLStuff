#version 410 core

// Per vertex
layout(location=0) in vec2 vertexPosition;
layout(location=1) in vec2 textureCoordinates;

// per instance
layout(location=2) in vec3 billboardPosition;

out vec2 UV;
out vec3 finalVertexPos;

uniform mat4 viewMatrix;
uniform mat4 viewProjection;

void main() {
    vec3 cameraRight = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 cameraUp = vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    vec2 billboardSize = vec2(3, 7);

    vec3 vertexPositionWorldspace =
        billboardPosition
        + cameraRight * vertexPosition.x * billboardSize.x
        + cameraUp * vertexPosition.y * billboardSize.y;
    
    gl_Position = viewProjection * vec4(vertexPositionWorldspace, 1);

    finalVertexPos = vertexPositionWorldspace;
    UV = textureCoordinates;
}