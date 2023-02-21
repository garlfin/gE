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

#define RAY_THICKNESS 0.1
#define ROUGHNESS 0.6

#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"
#include "../res/shdrinc/cubemap.glsl"

uniform uvec2 NormalTex;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragVelocity;

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
vec2 Hammersley(uint i, uint N);

void main()
{
    vec3 nor = pow(texture(sampler2D(NormalTex), TexCoord).rgb, vec3(1.0/2.2)) * 2 - 1;

    const vec3 normal = normalize(TBN * nor) * mix(-1, 1, gl_FrontFacing);
    const vec3 viewDir = normalize(FragPos - Position);
    vec3 rayDir = ImportanceSampleGGX(Hammersley(int(interleavedGradientSample * 1024), 1024), reflect(viewDir, normal), ROUGHNESS);
    vec2 reflection = vec2(-1);

#ifndef FORWARD
    vec3 rayPos = FragPos + interleavedGradientSample * rayDir * 0.1;
    if(dot(rayDir, normalize(Normal) * mix(-1, 1, gl_FrontFacing)) >= 0) reflection = CastRay(rayPos, rayDir, int(mix(150.0, 50.0, ROUGHNESS)), 10, RAY_MODE_ACCURATE, mix(0.01, 0.1, ROUGHNESS));
    FragColor = mix(SampleCubemap(Cubemaps[0], rayDir), texture(FrameColorTex, reflection), reflection.x >= 0 ? 1 : 0);
#else
    FragColor = SampleCubemap(Cubemaps[0], rayDir);
#endif

    vec4 fragNew = ViewPositions[0];
    fragNew /= fragNew.w;
    fragNew.xy = fragNew.xy * 0.5 + 0.5;

    vec4 fragOld = ViewPositions[1];
    fragOld /= fragOld.w;
    fragOld.xy = fragOld.xy * 0.5 + 0.5;

    FragVelocity = fragNew - fragOld;
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.32830643653869633e-10; // / 0x100000000
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
    return tangent * H.x + bitangent * H.y + N * H.z;
}

