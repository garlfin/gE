#version 460 core
#extension GL_ARB_bindless_texture: enable

uniform uvec2 Skybox;
in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    //FragColor = vec4(1);
    FragColor = texture(samplerCube(Skybox), TexCoords);
    FragColor = pow(FragColor, vec4(1.0/2.2));
}