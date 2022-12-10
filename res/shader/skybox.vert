#version 460 core
#include "../res/shader/camera.glsl"

layout(location = 0) in vec3 vPos;

layout(std140, binding = 0) uniform SceneInfo
{
    Camera cam;
};

out vec3 TexCoords;

void main()
{
    TexCoords = vPos;
    gl_Position = (cam.Projection * cam.View * vec4(vPos, 1.0)).xyww;
}

