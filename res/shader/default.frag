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

out vec4 FragColor;

#define SAMPLE_COUNT 4
#define CAM_SIZE 10
#define MAX_SEARCH 0.1
#define LIGHT_SIZE 0.05

float linearizeDepth(float z, vec2 planes)
{
    return 2.0 * planes.x * planes.y / (planes.y + planes.x - (z * 2 - 1) * (planes.y - planes.x));
}

float calcPenumbra(vec3 projCoord)
{
    float avgDepth = 0;
    uint count = 0;

    for(int i = 0; i < SAMPLE_COUNT; i++)
    {
        float sampleDepth = texture(sampler2D(ShadowTex), projCoord.xy + vogelDiskSample(i, SAMPLE_COUNT, interleavedGradientNoise() * 2 * 3.14159) * MAX_SEARCH / CAM_SIZE).r;
        sampleDepth = linearizeDepth(sampleDepth, vec2(0.01, 100));

        if(sampleDepth > projCoord.z) continue;

        count++;
        avgDepth += sampleDepth;
    }

    if(count == 0) return 0;
    avgDepth /= float(count);

    return avgDepth;//(FragPosLightSpace.z - avgDepth) * LIGHT_SIZE / avgDepth;
}


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
    vec3 normal = normalize(Normal);

    float shadow = calcShadow();

    float light = dot(normal, SunInfo.xyz) * 0.5 + 0.5;
    light = min(shadow * 0.5 + 0.5, light);
    light *= light;

    vec3 spec = pow(max(dot(reflect(-SunInfo.xyz, normal), normalize(Position - FragPos)), 0.0), 256.0).rrr;
    spec = mix(vec3(0), spec, shadow);

    FragColor = vec4(texture(sampler2D(Albedo), TexCoord).rgb * light + spec, 1);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}
