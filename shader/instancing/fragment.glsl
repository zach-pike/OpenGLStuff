#version 410 core

out vec3 color;

uniform sampler2D myTexture;
in vec2 UV;

in vec3 finalVertexPos;

uniform vec3 cameraPosition;

vec3 lerp(vec3 a, vec3 b, float t) {
    return a + (b - a) * t;
}

uniform float fogMin;
uniform float fogMax;

uniform vec3 fogColor;

void main() {
    vec3 preColor = texture(myTexture, vec2(UV.x, 1 - UV.y)).rgb;

    float fragmentDistance = distance(cameraPosition, finalVertexPos);

    float t = (fragmentDistance - fogMin) / (fogMax - fogMin);
    color = lerp(preColor, fogColor, clamp(t, 0, 1));
}