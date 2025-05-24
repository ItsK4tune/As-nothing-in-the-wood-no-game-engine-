#version 460 core

in vec3 gColor;
in vec3 gWorldPos;

#define MAX_SOUND_POINTS 100

uniform bool useColor;
uniform int soundCount;
uniform vec3 soundPositions[MAX_SOUND_POINTS];
uniform float soundRadii[MAX_SOUND_POINTS];
uniform float soundMaxRadii[MAX_SOUND_POINTS];

out vec4 FragColor;

void main()
{
    vec3 color = gColor;

    if (!useColor) {
        for (int i = 0; i < soundCount; ++i) {
            float dist = length(soundPositions[i] - gWorldPos);

            if (dist < soundRadii[i]) {
                float ratio = clamp(dist / soundMaxRadii[i], 0.0, 1.0);
                float brightness = pow(1.0 - ratio, 0.8);
                color = max(color, vec3(brightness));
            }
        }
    }

    FragColor = vec4(color, 1.0);
}
