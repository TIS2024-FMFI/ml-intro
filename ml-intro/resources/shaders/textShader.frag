#version 330 core
in vec2 TexCoords;
in float inVal;

out vec4 FragColor;

uniform float weight;
uniform sampler2D textureAtlas;

void main() {
    //vec2 tmpUV0 = vec2(TexCoords.x, 1.0-TexCoords.y);
    //vec2 tmpUV1 = (tmpUV0 - .5)*1.2 + .5;
    //vec2 tmpUV2 = (tmpUV0 - .5)*1.4 + .5;

    float f = texture(textureAtlas, TexCoords).x;
    //vec3 col = vec3(clamp(-weight, 0., 1.), clamp(weight, 0., 1.), .0);
        
    //float out1 = texture(text, tmpUV0).r;
    //float out2 = texture(text, tmpUV2).r;
    //float outline = clamp(clamp(out1 + out2, 0., 1.) - sampled.w, 0., 1. );
    //if (sampled.w + outline == 0.) discard;
    //FragColor = (vec4(col, 1.) * sampled.w) + vec4(0., 0., 0., outline);
    if (f <= 0.1) discard;
    vec3 col = vec3(clamp(-inVal, 0., 1.), clamp(inVal, 0., 1.), 0.2);
    FragColor = vec4(col, 1.);
}