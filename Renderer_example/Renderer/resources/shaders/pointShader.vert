#version 330 core
layout (location = 0) in vec3 aPos;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float pointSize;


void main() {
    vec4 viewSpacePos = view * vec4(aPos, 1.0);
    gl_Position = projection * viewSpacePos;

    gl_PointSize = pointSize / -viewSpacePos.z;
}