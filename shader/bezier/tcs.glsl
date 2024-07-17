#version 410 core

layout(vertices = 4) out;

void main() {
    // per vertex code
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Same for all verts in a patch
    gl_TessLevelOuter[0] = 1.0;
    gl_TessLevelOuter[1] = 20.0;
}