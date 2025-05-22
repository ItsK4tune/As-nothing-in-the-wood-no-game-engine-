#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vColor;
out vec3 vWorldPos;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;

    vColor = aColor;

    vec4 pos = projection * view * worldPos;
    pos.z -= 1e-3 * pos.w;
    gl_Position = pos;
}