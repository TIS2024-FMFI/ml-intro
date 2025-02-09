#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float activation;

out vec4 vertexColor;

uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;


void main() {
    vec4 viewSpacePos = view * vec4(aPos, 1.0);
    gl_Position = projection * viewSpacePos;

    gl_PointSize = pointSize / -viewSpacePos.z;
    vec3 col = vec3(clamp(-activation, 0., 1.), clamp(activation, 0., 1.), 0.);
    vertexColor = vec4(col, 1.0);
}