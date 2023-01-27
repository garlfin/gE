#version 460 core
#extension GL_ARB_bindless_texture: enable
#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"
#include "../res/shader/noise.glsl"

layout(early_fragment_tests) in;

uniform uvec2 Albedo;

in FragInfo
{
    vec3 FragPos;
    uvec4 InstanceInfo;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    mat3 TBN;
};

#include "../res/shader/ray.glsl"
#include "../res/shader/ssao.glsl"

out vec4 FragColor;

#define SAMPLE_COUNT 8
#define CAM_SIZE 10
#define LIGHT_SIZE 0.5

float calcPenumbra(vec3 projCoord)
{
    float avgDepth = 30;
    uint count = 0;

    float searchSize = max(0, projCoord.z - linearizeDepth(texture(sampler2D(ShadowTex), projCoord.xy).r, vec2(0.01, 30)));
    if(searchSize < 0.01) return 0.01;

    #pragma unroll
    for(int i = 0; i < SAMPLE_COUNT; i++)
    {
        float sampleDepth = texture(sampler2D(ShadowTex), projCoord.xy + vogelDiskSample(i, SAMPLE_COUNT, interleavedGradientSample * 2 * PI) * searchSize / CAM_SIZE).r;
        sampleDepth += linearizeDepth(sampleDepth, vec2(0.005, 30));

        if(sampleDepth > projCoord.z) continue;

        count++;
        avgDepth = min(avgDepth, sampleDepth);
    }
    if(count == 0) return 0;

    //avgDepth /= count;
    return max((projCoord.z - avgDepth) * LIGHT_SIZE / avgDepth, 0.01);
}


float calcShadow()
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float penumbra = calcPenumbra(projCoords);
    if(penumbra == 0) return 1;

    float shadow = 0;
    #pragma unroll
    for(int i = 0; i < SAMPLE_COUNT; i++)
    {
        vec2 vogelSample = vogelDiskSample(i, SAMPLE_COUNT, interleavedGradientNoise() * 2 * 3.14159) * penumbra;
        shadow += texture(sampler2DShadow(ShadowTex), vec3(projCoords.xy + vogelSample / CAM_SIZE, projCoords.z - 0.005));
    }

    return shadow / SAMPLE_COUNT;
}

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 rayDir = normalize(SunInfo.xyz) * (1.0 / 25);
    vec3 rayPos = FragPos + rayDir * (1 + interleavedGradientNoise());

    float shadow = calcShadow();
    vec2 occlusion = castRay(rayPos, rayDir, 1.0, 25, RAY_MODE_CHEAP);

    float light = clamp(pow(dot(normal, normalize(SunInfo.xyz)) * 0.5 + 0.5, 2), 0, 1);
    light = min(min(shadow, occlusion.x == -1 ? 1 : 0) * 0.5 + 0.5, light);
    light = mix(0.1, 1, light);

    vec3 spec = pow(max(dot(reflect(-SunInfo.xyz, normal), normalize(Position - FragPos)), 0.0), 256.0).rrr;
    spec = mix(vec3(0), spec, shadow);

    FragColor = vec4(texture(sampler2D(Albedo), TexCoord).rgb * light + spec, 1);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
    FragColor *= ComputeSSAO();
}
