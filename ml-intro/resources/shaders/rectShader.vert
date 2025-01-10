#version 330 core
layout (location = 0) in vec2 vPos; // vertex position
layout (location = 1) in vec3 mPos; // model world position

uniform mat4 projection;
uniform mat4 view;

void main() {
    mat4 translation = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        mPos.xyz, 1.0
    );

    mat4 billboardRotation = mat4(
        view[0][0], view[1][0], view[2][0], 0.0,
        view[0][1], view[1][1], view[2][1], 0.0,
        view[0][2], view[1][2], view[2][2], 0.0,
        0.0,        0.0,        0.0,        1.0
    );

    mat4 model = translation * billboardRotation;

    mat4 mvp = projection * view * model;

    gl_Position = mvp * vec4(vPos, 0.0005, 1.0);
}