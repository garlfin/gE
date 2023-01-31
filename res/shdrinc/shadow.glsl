#ifndef INCLUDE_SHADOW
#define INCLUDE_SHADOW 1

#ifndef SUN_SIZE
#define SUN_SIZE 0.2
#endif
#ifndef SHADOW_SAMPLES
#define SHADOW_SAMPLES 8
#endif
#ifndef PENUMBRA_MIN
#define PENUMBRA_MIN 0.0001
#endif
#ifndef SHADOW_BIAS
#define SHADOW_BIAS 0.005
#endif

#define PI2 6.28318
#define PENUMBRA_THRESHOLD 0.01
#define SHADOW_MAX 50

float CalculateShadow();
float _calculatePenumbra(float searchSize);
float _linearizeDepthOrtho(float z, vec2 p);

float CalculateShadow()
{
    const vec3 shadowCoord = vec3(FragPosLightSpace.xy / FragPosLightSpace.w, FragPosLightSpace.z) * 0.5 + 0.5;
    const float penumbra = _calculatePenumbra(0.2);
    float shadowAvg = 0;

    if(penumbra == 0) return 1;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        float shadowDepth = texture(sampler2D(ShadowTex), shadowCoord.xy + vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * penumbra / SunInfo.w).r;
        //shadowDepth = _linearizeDepth(shadowDepth * 0.5 + 0.5, vec2(0.01, SHADOW_MAX));
        shadowAvg += shadowDepth > shadowCoord.z - SHADOW_BIAS ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float _calculatePenumbra(float searchSize)
{
    vec3 shadowCoord = vec3(FragPosLightSpace.xyz / FragPosLightSpace.w) * 0.5 + 0.5;
    shadowCoord.z = shadowCoord.z * SHADOW_MAX;

    float shadowAvg = 0;
    uint count = 0;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        float shadowSample = texture(sampler2D(ShadowTex), shadowCoord.xy + vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * searchSize / SunInfo.w).r;
        shadowSample = shadowSample * SHADOW_MAX;

        if(shadowSample > shadowCoord.z - SHADOW_BIAS) continue;

        count++;
        shadowAvg += shadowSample;
    }

    if(count == 0) return 0.0;
    shadowAvg /= count;

    return max(SUN_SIZE * (shadowCoord.z - shadowAvg) / shadowAvg, PENUMBRA_MIN);
}

float _linearizeDepthOrtho(float z, vec2 p)
{
    return z * -2 / (p.y - p.x) - (p.y + p.x) / (p.y - p.x);
}

#endif