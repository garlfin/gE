#extension GL_ARB_bindless_texture: enable

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"

layout(early_fragment_tests) in;

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

#define RAY_THICKNESS 0.3
#define ROUGHNESS 0.0

#include "../res/shdrinc/taa.glsl"
#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"
#include "../res/shdrinc/cubemap.glsl"
#include "../res/shdrinc/ssao.glsl"
#include "../res/shdrinc/shadow.glsl"

layout(bindless_sampler) uniform sampler2D NormalTex;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec2 FragVelocity;

void main()
{
    vec3 nor = pow(texture(NormalTex, TexCoord).rgb, vec3(1.0/2.2)) * 2 - 1;

    const vec3 normal = normalize(TBN * nor);
    const vec3 viewDir = normalize(FragPos - CamPos);
    vec3 rayDir = ImportanceSampleGGX(Hammersley(int(interleavedGradientSample * 1024), 1024), reflect(viewDir, normal), ROUGHNESS);

#ifndef FORWARD
    vec2 reflection = vec2(-1);
    int rayCount = int(mix(50.0, 20.0, ROUGHNESS));
    vec3 rayPos = FragPos + interleavedGradientSample * (normalize(rayDir) * 10 / rayCount);
    if(dot(rayDir, normalize(Normal)) >= 0) reflection = CastRay(rayPos, rayDir, rayCount, 10, RAY_MODE_ACCURATE, mix(0.01, 0.1, ROUGHNESS));
    FragColor = mix(SampleCubemap(Cubemaps[0], rayDir), texture(FrameColorTex, reflection), reflection.x >= 0 ? 1 : 0);
#else
    FragColor = SampleCubemap(Cubemaps[0], rayDir);
#endif

    FragColor *= CalculateSSAO(normal);
    FragColor.a = 1;

    FragVelocity = ((ViewPositions[0].xy / ViewPositions[0].w * 0.5 + 0.5) - (ViewPositions[1].xy / ViewPositions[1].w * 0.5 + 0.5));//_worldToScreenNoJitter(FragPos).xyxy - _worldToScreenPrev(FragPos).xyxy;
}