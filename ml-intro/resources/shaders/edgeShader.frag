#version 330 core

in vec4  fragColor;
in float fragVal;
out vec4 FragColor;

uniform float t;

#define TWOPI 6.2831853

void main() {
    float f = sin((fragVal-t)*TWOPI) * .5 +.5;
    f = 1.-pow(f, 2.);
    vec4 col = fragColor * f;
    
    float val = max(abs(fragColor.x), abs(fragColor.y));

    FragColor = vec4(col.xyz, val);

}
