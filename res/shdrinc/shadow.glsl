#ifndef INCLUDE_SHADOW
#define INCLUDE_SHADOW 1


#ifndef SUN_SIZE
#define SUN_SIZE 0.5
#endif
#ifndef SHADOW_SAMPLES
#define SHADOW_SAMPLES 16
#endif
#ifndef PENUMBRA_MIN
#define PENUMBRA_MIN 0.01
#endif
#ifndef SHADOW_BIAS
#define SHADOW_BIAS 0.0005
#endif
#ifndef SEARCH_SIZE
#define SEARCH_SIZE 0.4
#endif

#define PENUMBRA_THRESHOLD 0.01
#define SHADOW_MAX 50

//#define SHADOW_MODE_MIN 1

float CalculateShadow();
float CalculateShadow(float radius);
float _calculatePenumbra(float searchSize);
float _linearizeDepthOrtho(float z, vec2 p);

float CalculateShadow()
{
    const vec3 shadowCoord = vec3(FragPosLightSpace.xy / FragPosLightSpace.w, FragPosLightSpace.z) * 0.5 + 0.5;
    const float penumbra = min(_calculatePenumbra(SEARCH_SIZE), SEARCH_SIZE);

    float shadowAvg = 0;

    if(penumbra == 0) return 1;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        vec2 offset = vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * penumbra;
        float shadowDepth = texture(sampler2D(ShadowTex), shadowCoord.xy + offset / SunInfo.w ).r;
        shadowAvg += shadowDepth > shadowCoord.z - SHADOW_BIAS ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float CalculateShadow(float radius)
{
    const vec3 shadowCoord = vec3(FragPosLightSpace.xy / FragPosLightSpace.w, FragPosLightSpace.z) * 0.5 + 0.5;
    float shadowAvg = 0;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        float shadowDepth = texture(sampler2D(ShadowTex), shadowCoord.xy + vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * radius / SunInfo.w).r;
        shadowAvg += shadowDepth > shadowCoord.z - SHADOW_BIAS ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float _calculatePenumbra(float searchSize)
{
    vec3 shadowCoord = vec3(FragPosLightSpace.xyz / FragPosLightSpace.w) * 0.5 + 0.5;
    shadowCoord.z *= SHADOW_MAX;

    #ifdef SHADOW_MODE_MIN
    float shadowAvg = SHADOW_MAX;
    #else
    float shadowAvg = 0;
    #endif
    uint count = 0;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        vec2 offset = vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * SEARCH_SIZE;
        float shadowSample = texture(sampler2D(ShadowTex), shadowCoord.xy + offset / SunInfo.w).r;
        shadowSample *= SHADOW_MAX;//(SHADOW_BIAS + (TBN * vec3(offset, 0)).z / SHADOW_MAX);

        if(shadowSample > shadowCoord.z - SHADOW_BIAS * SHADOW_MAX) continue;

        count++;
    #ifdef SHADOW_MODE_MIN
        shadowAvg = min(shadowSample, shadowAvg);
    #else
        shadowAvg += shadowSample;
    #endif
    }

    if(count == 0) return 0.0;
#ifndef SHADOW_MODE_MIN
    shadowAvg /= count;
#endif

    return max(SUN_SIZE * (shadowCoord.z - shadowAvg) / shadowAvg, PENUMBRA_MIN);
}

float _linearizeDepthOrtho(float z, vec2 p)
{
    return z * -2 / (p.y - p.x) - (p.y + p.x) / (p.y - p.x);
}

#endif