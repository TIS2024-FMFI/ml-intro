#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
    //FragColor = vec4(1., 0., 0., 1.);
    vec2 uv = 2. * gl_PointCoord - vec2(1.);
    
    float d = dot(uv, uv);

    float a = step(d, 0.5);
    if (a == 0.) discard;
    vec3 col = mix(color.xyz, vec3(1.), step(d, 0.25));
    FragColor = vec4(col, a);
}