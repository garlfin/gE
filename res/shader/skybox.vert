#extension GL_NV_viewport_array : enable
#extension GL_NV_viewport_array2 : enable
#extension GL_ARB_shader_viewport_layer_array : enable
#extension GL_AMD_vertex_shader_layer : enable
#extension GL_ARB_bindless_texture: enable
#include "../res/shader/camera.glsl"

layout(location = 0) in vec3 vPos;

out vec3 TexCoords;

void main()
{
    gl_Layer = int(gl_InstanceID);
    TexCoords = vPos;
    gl_Position = (Projection * mat4(mat3(GetView(Position, gl_Layer))) * vec4(vPos, 1.0)).xyww;
}

