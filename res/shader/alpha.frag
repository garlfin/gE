#extension GL_ARB_bindless_texture: enable

layout(bindless_sampler) uniform sampler2D Albedo;

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"
#include "../res/shdrinc/noise.glsl"

in FragInfo
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
    if(texture(Albedo, TexCoord).a < interleavedGradientNoise(gl_FragCoord.xy, Stage == STAGE_SHADOW ? 0 : 1)) discard;
}