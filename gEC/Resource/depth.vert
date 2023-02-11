#extension GL_ARB_bindless_texture: enable
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

out vec2 TexCoord;

void main()
{
    TexCoord = vUV;
    gl_Position = Projection * View * Model[gl_InstanceID] * vec4(vPos, 1.0);
}