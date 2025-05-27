#version 460 core

in vec3 vColor;
in vec3 vWorldPos;

#define MAX_SOUND_POINTS 100

uniform bool useColor;
uniform int soundCount;
uniform vec3 soundPositions[MAX_SOUND_POINTS];
uniform vec3 soundColors[MAX_SOUND_POINTS];
uniform float soundRadii[MAX_SOUND_POINTS];
uniform float soundMaxRadii[MAX_SOUND_POINTS];
uniform bool soundIsGrowing[MAX_SOUND_POINTS];

out vec4 FragColor;

void main()
{
    vec3 color = vColor;

    if (!useColor) {
        for (int i = 0; i < soundCount; ++i) {
            float dist = length(soundPositions[i] - vWorldPos);
            float delta = abs(dist - soundRadii[i]);

            if (delta < 0.005 && soundIsGrowing[i]) {
                float ratio = clamp(soundRadii[i] / soundMaxRadii[i], 0.0, 1.0);
                float brightness = pow(1.0 - ratio, 0.8);
                color = max(color, vec3(soundColors[i] * brightness));
            }
        }
    }

    FragColor = vec4(color, 1.0);
}
