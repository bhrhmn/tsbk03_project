#version 150

in vec2 texCoord;
uniform sampler2D texUnit;

out vec4 out_Color;



void main(void)
{
    // Only showing light values above 1.0
    //out_Color = texture(texUnit, texCoord);
    out_Color = max(vec4(0.0, 0.0, 0.0, 0.0), texture(texUnit, texCoord) - vec4(1.0, 1.0, 1.0, 0.0));
}


