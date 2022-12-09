#version 460 core
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
layout(location = 2) in vec3 vNor;
layout(location = 3) in vec3 vTan;
layout(location = 4) in vec3 vCol;

out FragInfo
{
    vec3 FragPos;
    uvec4 InstanceInfo;
    vec3 Normal;
    vec2 TexCoord;
};

layout(std140, binding = 0) uniform SceneInfo
{
    Camera cam;
};

layout(std140, binding = 1) uniform ObjectsInfo
{
    ObjectInfo Objects;
};

void main() {
    FragPos = vPos;
    Normal = normalize(vNor);
    TexCoord = vUV;

    gl_Position = cam.Projection * cam.View * Objects.Model[gl_InstanceID] * vec4(vPos, 1.0);
}