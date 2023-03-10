#ifndef INCLUDE_SHADOW
#define INCLUDE_SHADOW 1

#define PI 3.14159

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

#define PENUMBRA_THRESHOLD 0.1
#define SHADOW_MAX (SunInfo.w * 2)

//#define SHADOW_MODE_MIN

struct DirectionalLight
{
    vec3 Dir;
    float Radius;
    float SearchRadius;
    vec3 Color;
    sampler2D ShadowMap;
    mat4 ProjViewMatrix;
};

float CalculateShadowPCSS(in const DirectionalLight light);
float CalculateShadow(in const DirectionalLight light);
float _calculatePenumbra(in const DirectionalLight light);

float CalculateShadowPCSS(in const DirectionalLight light)
{
    const vec3 shadowCoord = FragPosLightSpace.xyz / FragPosLightSpace.w * 0.5 + 0.5;
    const float penumbra = _calculatePenumbra(light);

    float shadowAvg = 0;

    if(penumbra == 0) return 1;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        vec2 offset = vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * penumbra;
        float shadowDepth = texture(light.ShadowMap, shadowCoord.xy + offset / SunInfo.w).r;
        shadowAvg += shadowDepth > (shadowCoord.z - SHADOW_BIAS) ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float CalculateShadow(in const DirectionalLight light)
{
    const vec3 shadowCoord = vec3(FragPosLightSpace.xy / FragPosLightSpace.w, FragPosLightSpace.z) * 0.5 + 0.5;
    float shadowAvg = 0;

    for(uint i = 0; i < SHADOW_SAMPLES; i++)
    {
        float shadowDepth = texture(light.ShadowMap, shadowCoord.xy + vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * light.Radius / SunInfo.w).r;
        shadowAvg += shadowDepth > shadowCoord.z - SHADOW_BIAS / SHADOW_MAX ? 1 : 0;
    }

    return shadowAvg / SHADOW_SAMPLES;
}

float _calculatePenumbra(in const DirectionalLight light)
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
        vec2 offset = vogelDiskSample(i, SHADOW_SAMPLES, interleavedGradientSample * PI2) * light.SearchRadius;
        float shadowSample = texture(light.ShadowMap, shadowCoord.xy + offset / SunInfo.w).r;
        shadowSample *= SHADOW_MAX;//(SHADOW_BIAS + (TBN * vec3(offset, 0)).z / SHADOW_MAX);

        if(shadowSample > shadowCoord.z - SHADOW_BIAS * SHADOW_MAX) continue;

        count++;
    #ifdef SHADOW_MODE_MIN
        shadowAvg = min(shadowSample, shadowAvg);
    #else
        shadowAvg += shadowSample;
    #endif
    }

    if(count == 0) return 0;
#ifndef SHADOW_MODE_MIN
    shadowAvg /= count;
#endif

    return min((shadowCoord.z - shadowAvg) * light.Radius, light.SearchRadius) + PENUMBRA_MIN;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float RadicalInverse_VdC(uint bits)
{
bits = (bits << 16u) | (bits >> 16u);
bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
float a = roughness * roughness;
float phi = 2.0 * 3.14159 * Xi.x;
float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

vec3 H;
H.x = cos(phi) * sinTheta;
H.y = sin(phi) * sinTheta;
H.z = cosTheta;

vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
vec3 tangent = normalize(cross(up, N));
vec3 bitangent = cross(N, tangent);
return normalize(tangent * H.x + bitangent * H.y + N * H.z);
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
float k = (roughness * roughness) / 2.0;
return NdotV / (NdotV * (1.0 - k) + k);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
float a      = roughness*roughness;
float a2     = a*a;
float NdotH  = max(dot(N, H), 0.0);
float NdotH2 = NdotH*NdotH;

float num   = a2;
float denom = (NdotH2 * (a2 - 1.0) + 1.0);
denom = PI * denom * denom;

return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
float NdotV = max(dot(N, V), 0.0);
float NdotL = max(dot(N, L), 0.0);
float ggx2  = GeometrySchlickGGX(NdotV, roughness);
float ggx1  = GeometrySchlickGGX(NdotL, roughness);

return ggx1 * ggx2;
}

vec3 CalculateLight(vec3 normal, vec3 flatNormal, vec3 view, vec3 albedo, vec2 surface, vec3 f0, float shadow) // surface(rough, metal)
{
    const vec3 light = normalize(SunInfo.xyz);
    const vec3 halfLight = normalize(view + light);
    const vec3 radiance = vec3(3);

    float NDF = DistributionGGX(normal, halfLight, surface.x);
    float G = GeometrySmith(normal, view, light, surface.x);
    vec3 F = fresnelSchlick(max(dot(halfLight, view), 0.0), f0);
    vec3 kD = (vec3(1.0) - F) * (1.0 - surface.y);

    vec3 specular = (NDF * G * F) / (4.0 * max(dot(normal, view), 0.0) * max(dot(normal, light), 0.0) + 0.0001) * shadow;

    float dots = max(min(dot(normal, light), dot(flatNormal, light)), 0.0);
    return (kD * albedo / PI + specular) * radiance * min(dots, shadow);
}

#endif