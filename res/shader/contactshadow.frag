#version 460 core
#extension GL_ARB_bindless_texture: enable

layout(early_fragment_tests) in;

in FragInfo
{
    vec3 FragPos;
    uvec4 InstanceInfo;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    mat3 TBN;
};

uniform uvec2 Albedo;

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"
#include "../res/shader/ray.glsl"
#include "../res/shader/ssao.glsl"


out vec4 FragColor;

#define ITERATIONS 25
#define MAX_LENGTH 0.2
#define SAMPLE_COUNT 4
#define CAM_SIZE 10
#define MAX_SEARCH 0.1
#define LIGHT_SIZE 0.05

float calcShadow()
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0;
    for(int i = 0; i < SAMPLE_COUNT; i++)
    shadow += texture(sampler2DShadow(ShadowTex), vec3(projCoords.xy + vogelDiskSample(i, SAMPLE_COUNT, interleavedGradientNoise() * 2 * 3.14159) * LIGHT_SIZE / CAM_SIZE, projCoords.z - 0.0005));

    return shadow / SAMPLE_COUNT;
}

void main()
{
    const vec3 rayDir = normalize(SunInfo.xyz) * (MAX_LENGTH / ITERATIONS);
    const vec3 normal = normalize(Normal);
    vec3 rayPos = FragPos + rayDir * (1 + interleavedGradientNoise());

    float shadow = calcShadow();
    vec2 occlusion = castRay(rayPos, rayDir, MAX_LENGTH, ITERATIONS, RAY_MODE_CHEAP);

    float light = clamp(pow(dot(normal, normalize(SunInfo.xyz)) * 0.5 + 0.5, 2), 0, 1);
    light = min(min(shadow, occlusion.x == -1 ? 1 : 0) * 0.5 + 0.5, light);
    light = mix(0.1, 1, light);

    vec3 spec = pow(max(dot(reflect(-SunInfo.xyz, normal), normalize(Position - FragPos)), 0.0), 256.0).rrr;
    spec = mix(vec3(0), spec, shadow);

    FragColor = vec4(texture(sampler2D(Albedo), TexCoord).rgb * light + spec, 1);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
    FragColor *= ComputeSSAO();
}


