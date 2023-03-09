#extension GL_ARB_bindless_texture: enable
#extension GL_NV_viewport_array : enable
#extension GL_NV_viewport_array2 : enable
#extension GL_ARB_shader_viewport_layer_array : enable
#extension GL_AMD_vertex_shader_layer : enable
#include "../res/shader/camera.glsl"
#include "../res/shader/objectinfo.glsl"
#include "../res/shader/demowindow.glsl"
#include "../res/shdrinc/taa.glsl"

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

out vec2 TexCoord;

void main()
{
    gl_Layer = int(gl_InstanceID / ObjectCount);
    TexCoord = vUV;

    gl_Position = Projection * GetView(CamPos, gl_Layer) * Model[gl_InstanceID % ObjectCount] * vec4(vPos, 1.0);
}