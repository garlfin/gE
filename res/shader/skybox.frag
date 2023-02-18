#extension GL_ARB_bindless_texture: enable

in vec3 FragPos;

#include "../res/shdrinc/cubemap.glsl"

in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    FragColor = textureLod(SkyboxTex, TexCoords, 0);
}