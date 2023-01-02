#version 460 core
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"

layout(location = 0) in vec3 vPos;

void main()
{
    gl_Position = Projection * View * Model[gl_InstanceID] * vec4(vPos, 1.0);
}