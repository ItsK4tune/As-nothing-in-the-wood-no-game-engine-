#version 460 core

in vec3 vColor;
in vec3 vWorldPos;

uniform bool useColor;
uniform vec3 cameraPos;
uniform float soundRadius;

out vec4 FragColor;

void main()
{
    float distance = length(cameraPos - vWorldPos);

    vec3 color;
    if (useColor) {
        color = vColor;
    } else {
        float delta = abs(distance - soundRadius);
        if (delta < 0.05) {
            float brightness = 1.0 - clamp(delta / 0.05, 0.0, 1.0);
            color = vec3(brightness);
        } else {
            color = vColor;
        }
    }

    FragColor = vec4(color, 1.0);
}
