#extension GL_ARB_bindless_texture: enable

in vec3 FragPos;

#include "../res/shdrinc/cubemap.glsl"

in vec3 TexCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragVelocity;

void main()
{
    FragColor = textureLod(SkyboxTex, TexCoords, 0);
    FragVelocity = vec4(0);
}