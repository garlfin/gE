#version 460 core
#extension GL_ARB_TEXTURE_BINDLESS: enable

uniform samplerCube Skybox;

in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    FragColor = texture(Skybox, TexCoords);
    FragColor = pow(FragColor, vec4(1/2.2));
}