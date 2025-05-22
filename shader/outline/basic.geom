#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec3 vColor[];
in vec3 vWorldPos[];

out vec3 gColor;
out vec3 gWorldPos;

void main() {
    // Line 1: from vertex 0 to vertex 1
    gColor = vColor[0];
    gWorldPos = vWorldPos[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gColor = vColor[1];
    gWorldPos = vWorldPos[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();

    // Line 2: from vertex 1 to vertex 2
    gColor = vColor[1];
    gWorldPos = vWorldPos[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gColor = vColor[2];
    gWorldPos = vWorldPos[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
}
