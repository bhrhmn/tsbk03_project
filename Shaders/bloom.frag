#version 150

in vec2 texCoord;
uniform sampler2D texUnit;
uniform sampler2D texUnitBloom;

out vec4 out_Color;


void main(void)
{
    //out_Color = texture(texUnit, texCoord);
    // only blooming
    //out_Color = texture(texUnitBloom, texCoord);
    // combined
    out_Color = texture(texUnit, texCoord) + texture(texUnitBloom, texCoord);
}


