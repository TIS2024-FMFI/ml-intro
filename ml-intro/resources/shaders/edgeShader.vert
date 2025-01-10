#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aWeight;
layout(location = 2) in float aVal;

uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;
out float fragVal;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    fragColor = vec4(clamp(-aWeight, 0., 1.), clamp(aWeight, 0., 1.), 0., 1.);
    fragVal = aVal;
}
