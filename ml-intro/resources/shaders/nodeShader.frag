#version 330 core
in vec4 vertexColor;
in float isSquareNode;
out vec4 FragColor;


void main()
{
    vec2 uv = 2. * gl_PointCoord - vec2(1.);
    
    float d = dot(uv, uv);

    float a = step(d - isSquareNode*0.50, 1.0);
    if (a == 0.) discard;
    vec3 col = mix(vertexColor.xyz, vec3(1.), step(d, 0.5));
    FragColor = vec4(col, 1.);
    //FragColor = vec4(gl_PointCoord, 0., 1.);
    //FragColor = vec4(1., 0., 0., 1.);
}