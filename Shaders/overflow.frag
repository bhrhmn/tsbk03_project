#version 150

in vec2 texCoord;
uniform sampler2D texUnit;

out vec4 out_Color;

void main(void)
{
    // Only showing light values above 1.0
    vec4 texel = texture(texUnit, texCoord);
    float brightness = dot(texel.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        out_Color = texel;
    else
        out_Color = vec4(0.0, 0.0, 0.0, 1.0);
}


