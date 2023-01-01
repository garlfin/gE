#version 460 core
#extension GL_ARB_bindless_texture : enable
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"
#include "../res/shader/demowindow.glsl"

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
    vec4 FragPosLightSpace;
    mat3 TBN;
};

void main() {
    FragPos = (Model[gl_InstanceID] * vec4(vPos, 1.0)).xyz;
    Normal = normalize(mat3(NormalMatrix[gl_InstanceID]) * vNor);
    TexCoord = vUV;

    gl_Position = Projection * View * Model[gl_InstanceID] * vec4(vPos, 1.0);
    FragPosLightSpace = SunMatrix * vec4(FragPos, 1.0);

    TBN = mat3(vTan, normalize(cross(vTan, Normal)), Normal);
}