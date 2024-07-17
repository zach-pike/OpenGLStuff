#version 410 core

out vec4 color;

in vec2 outUV;

uniform sampler2D myTexture;

void main() {
    color = texture(myTexture, outUV);
}