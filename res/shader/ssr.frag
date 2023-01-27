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

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"
#include "../res/shader/ray.glsl"


out vec4 FragColor;

#define ITERATIONS 50
#define MAX_LENGTH 5.0
#define ROUGHNESS 0.1
#define PI 3.14159
#define VOGEL_SAMPLE 64

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
vec2 Hammersley(uint i, uint N);

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 viewDir = normalize(FragPos - Position);
    const vec2 screenUV = gl_FragCoord.xy / Info.xy;

    vec3 rayDir = ImportanceSampleGGX(Hammersley(int(interleavedGradientSample * VOGEL_SAMPLE), VOGEL_SAMPLE), reflect(viewDir, normal), ROUGHNESS);
    vec2 reflection = vec2(-1);

    vec3 rayPos = FragPos;
    reflection = castRay(rayPos, rayDir, MAX_LENGTH, ITERATIONS, RAY_MODE_EXPENSIVE);


    if (reflection.x == -1.0)
         FragColor = pow(texture(SkyboxTex, rayDir), vec4(1.0 / 2.2));
    else
         FragColor = texture(FrameColorTex, reflection);
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
    float a = roughness*roughness;
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;

    return normalize(sampleVec);
}


