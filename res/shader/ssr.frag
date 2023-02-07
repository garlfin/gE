#version 460 core
#extension GL_ARB_bindless_texture: enable

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"

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

#define RAY_THICKNESS 1.0
#define RAY_THRESHOLD 0.02

#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"

out vec4 FragColor;

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
vec2 Hammersley(uint i, uint N);

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 viewDir = normalize(FragPos - Position);

    vec3 rayDir = ImportanceSampleGGX(Hammersley(int(interleavedGradientSample * 256), 256), reflect(viewDir, normal), 0.1);
    vec3 RayPos = FragPos;
    vec2 reflection;

    //if(dot(rayDir, normal) < 0.03) reflection = vec2(-1); else
    reflection = CastRay(RayPos, rayDir, 150, 10, RAY_MODE_ACCURATE);

    FragColor = mix(pow(texture(SkyboxTex, rayDir), vec4(1.0 / 2.2)), texture(FrameColorTex, reflection), reflection.x >= 0 ? 1 : 0);
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
    return tangent * H.x + bitangent * H.y + N * H.z;
}

