#extension GL_ARB_bindless_texture : enable
#include "../res/shader/camera.glsl"

layout(location = 0) uniform sampler2D Tex;
out vec4 FragColor;

void main()
{
    FragColor = pow(texture(Tex, gl_FragCoord.xy / Info.xy), vec4(1.0 / 2.2));
}