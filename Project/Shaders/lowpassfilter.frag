#version 150

in vec2 texCoord;
uniform sampler2D texUnit;
uniform float windowWidth;
uniform float windowHeight;

out vec4 out_Color;


void main(void)
{
    
    
    //Filter with simple lowpass filter
    mat3 lowpass;
    lowpass[0] = vec3(1, 2, 1);
    lowpass[1] = vec3(2, 4, 2);
    lowpass[2] = vec3(1, 2, 1);

    // for fun
    // mat3 highpass;
    // highpass[0] = vec3(-1, -1, -1);
    // highpass[1] = vec3(-1, 17, -1);
    // highpass[2] = vec3(-1, -1, -1);

    vec4 res = vec4(0, 0, 0, 0);
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            vec2 coord = vec2(texCoord.x + i*windowWidth, texCoord.y + j*windowHeight);
            res += texture(texUnit, coord) * lowpass[i+1][j+1] / 16;
        }
    }

    out_Color = res;
    

    // Only showing light values above 1.0
    //out_Color = texture(texUnit, texCoord);
}


