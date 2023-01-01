#version 460 core
#extension GL_ARB_bindless_texture : enable

layout(location = 0) uniform sampler2D Tex;
out vec4 FragColor;

void main()
{
    FragColor = texture(Tex, gl_FragCoord.xy / vec2(1280, 720));
}