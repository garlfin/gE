#version 460 core
#extension GL_ARB_bindless_texture: enable
#include "../res/shader/demowindow.glsl"

in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    //FragColor = vec4(1);
    FragColor = texture(SkyboxTex, TexCoords);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}