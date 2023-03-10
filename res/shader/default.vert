#extension GL_NV_viewport_array : enable
#extension GL_NV_viewport_array2 : enable
#extension GL_ARB_shader_viewport_layer_array : enable
#extension GL_AMD_vertex_shader_layer : enable
#extension GL_ARB_bindless_texture : enable

#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"
#include "../res/shader/demowindow.glsl"
#include "../res/shdrinc/taa.glsl"

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

    mat2x4 ViewPositions;
};
void main()
{
    gl_Layer = int(gl_InstanceID / ObjectCount);
    mat3 normalMatrix = mat3(NormalMatrix[gl_InstanceID % ObjectCount]);
    FragPos = (Model[gl_InstanceID % ObjectCount] * vec4(vPos, 1.0)).xyz;
    Normal = normalize(normalMatrix * vNor);
    TexCoord = vUV;

    gl_Position = Projection * GetView(CamPos, gl_Layer) * Model[gl_InstanceID % ObjectCount] * vec4(vPos, 1.0);

    FragPosLightSpace = SunMatrix * vec4(FragPos, 1.0);

    vec3 tan = normalize(normalMatrix * vTan);
    TBN = mat3(tan, normalize(cross(Normal, tan)), Normal);

    ViewPositions[0] = gl_Position;
    ViewPositions[1] = PreviousProjection * PreviousView * PreviousModel[gl_InstanceID % ObjectCount] * vec4(vPos, 1.0);
    if(Stage != STAGE_SHADOW) gl_Position.xy += Jitter() * gl_Position.w;
}