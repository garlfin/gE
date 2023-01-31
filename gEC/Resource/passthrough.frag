#version 460 core
#extension GL_ARB_bindless_texture : enable
#include "../res/shader/camera.glsl"

layout(location = 0) uniform sampler2D Tex;
out vec4 FragColor;

void main()
{
    FragColor = texture(Tex, gl_FragCoord.xy / Info.xy);
}