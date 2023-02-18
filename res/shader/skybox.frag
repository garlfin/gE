#extension GL_ARB_bindless_texture: enable
#include "../res/shader/demowindow.glsl"

in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    FragColor = textureLod(SkyboxTex, TexCoords, 0);
}