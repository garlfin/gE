#ifndef INCLUDE_SHADOW
#define INCLUDE_SHADOW 1

#define LIGHT_DIRECTIONAL 1
#define LIGHT_POINT 2

#ifndef SHADOW_SAMPLES
#define SHADOW_SAMPLES 16
#endif
#ifndef PENUMBRA_MIN
#define PENUMBRA_MIN 0.01
#endif
#ifndef SHADOW_BIAS
#define SHADOW_BIAS 0.001
#endif
#ifndef SEARCH_SIZE
#define SEARCH_SIZE 0.4
#endif

#define PENUMBRA_THRESHOLD 0.1
#define SHADOW_MAX (SunInfo.w * 2)

//#define SHADOW_MODE_MIN 1

float CalculateShadow(uint lightType, float lightRadius);
float CalculateShadow(float radius);
float _calculatePenumbra(float searchSize, uint lightType, float lightSize);
float _linearizeDepthOrtho(float z, vec2 p);

float CalculateShadow(uint lightType, float lightRadius)
{
    const vec3 shadowCoord = vec3(FragPosLightSpace.xy / FragPosLightSpace.w, FragPosLightSpace.z) * 0.5 + 0.5;
    const float penumbra = _calculatePenumbra(SEARCH_SIZE, lightType, lightRadius);
       // return penumbra;
    float shadowAvg = 0;

    if(penumbra == 0) return 1;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        vec2 offset = vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * penumbra;
        float shadowDepth = texture(sampler2D(ShadowTex), shadowCoord.xy + offset / SunInfo.w).r;
        shadowAvg += shadowDepth > (shadowCoord.z - SHADOW_BIAS) ? 1 : 0;
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
        shadowAvg += shadowDepth > shadowCoord.z - SHADOW_BIAS / SHADOW_MAX ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float _calculatePenumbra(float searchSize, uint lightType, float lightSize)
{
    vec3 shadowCoord = FragPosLightSpace.xyz / FragPosLightSpace.w * 0.5 + 0.5;
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
        shadowAvg = min(shadowSample, shawdowAvg);
    #else
        shadowAvg += shadowSample;
    #endif
    }

    if(count == 0) return 0.0;
#ifndef SHADOW_MODE_MIN
    shadowAvg /= count;
#endif

    if(lightType == LIGHT_DIRECTIONAL)
        return min((shadowCoord.z - shadowAvg) * lightSize, searchSize) + PENUMBRA_MIN;
    else
        return min((shadowCoord.z - shadowAvg) * lightSize / shadowAvg, searchSize) + PENUMBRA_MIN;
}

#endif