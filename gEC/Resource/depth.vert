#version 460 core
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"

layout(location = 0) in vec3 vPos;

layout(std140, binding = 0) uniform SceneInfo
{
    Camera cam;
};

layout(std140, binding = 1) uniform ObjectsInfo
{
    ObjectInfo Objects;
};

void main()
{
    gl_Position = cam.Projection * cam.View * Objects.Model[gl_InstanceID] * vec4(vPos, 1.0);
}