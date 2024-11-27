#version 330 core
out vec4 FragColor;

uniform vec3 color;

void main()
{
    //FragColor = vec4(1., 0., 0., 1.);
    vec2 uv = 2. * gl_PointCoord - vec2(1.);

    float a = 1. - dot(uv, uv);
    FragColor = vec4(color, a);
}