#extension GL_ARB_bindless_texture: enable
#include "../res/shader/camera.glsl"

layout(location = 0) in vec3 vPos;

out vec3 TexCoords;

void main()
{
    TexCoords = vPos;
    gl_Position = (Projection * mat4(mat3(View)) * vec4(vPos, 1.0)).xyww;
}

