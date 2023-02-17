#extension GL_ARB_bindless_texture: enable

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"

layout(early_fragment_tests) in;

uniform uvec2 Albedo;
uniform uvec2 Roughness;
uniform uvec2 NormalTex;

in FragInfo
{
    vec3 FragPos;
    uvec4 InstanceInfo;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    mat3 TBN;
};

#define SHADOW_SAMPLES 16
#define PENUMBRA_MIN 0.05
#define SEARCH_SIZE 0.5
#define SHADOW_BIAS 0.001
#define RAY_THICKNESS 1.0
#define METALLIC 0.0
//#define SHADOW_MODE_MIN

#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"
#include "../res/shdrinc/shadow.glsl"
#include "../res/shdrinc/cubemap.glsl"

out vec4 FragColor;

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);
vec2 Hammersley(uint i, uint N);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    vec3 nor = pow(texture(sampler2D(NormalTex), TexCoord).rgb, vec3(1.0/2.2)) * 2 - 1;
    const vec3 normal = normalize(TBN * nor);
    const vec3 light = normalize(SunInfo.xyz);
    const vec3 incoming = normalize(Position - FragPos);

    const vec4 albedo = texture(sampler2D(Albedo), TexCoord);

    const float roughness = pow(texture(sampler2D(Roughness), TexCoord).r, 1.0/2.2);
    const vec3 f0 = mix(vec3(0.04), albedo.rgb, METALLIC);
    const vec3 kS = fresnelSchlickRoughness(max(0, dot(normal, incoming)), f0, roughness);
    const vec3 kD = (vec3(1) - kS) * (1 - METALLIC);

    vec3 rayDir = ImportanceSampleGGX(Hammersley(int(interleavedGradientSample * 256), 256), reflect(-incoming, normal), roughness);
    vec2 reflection = vec2(-1);

#ifndef FORWARD
    vec3 rayPos = FragPos;
    if(dot(rayDir, normalize(Normal)) >= 0) reflection = CastRay(rayPos, rayDir, int(mix(150, 10, roughness)), 10, RAY_MODE_ACCURATE, mix(0.01, 0.1, roughness));
#endif

    float ambient = max(dot(normal, light), 0);
    ambient = min(ambient, CalculateShadow(LIGHT_DIRECTIONAL, 0.05));

    vec2 brdf = texture(BRDFLut, vec2(clamp(dot(incoming, normal), 0, 1), roughness)).rg;
#ifndef FORWARD
    vec3 spec = mix(SampleCubemap(Cubemaps[0], reflect(-incoming, normal), roughness), texture(FrameColorTex, reflection), reflection.x < 0 ? 0 : 1).rgb;
#else
    vec3 spec = SampleCubemap(Cubemaps[0], reflect(-incoming, normal), roughness).rgb;
#endif
    spec += vec3(pow(max(0, dot(reflect(light, normal), -incoming)), pow(2 - roughness, 16))) * ambient;
    spec *= kS * brdf.x + brdf.y;

    FragColor = vec4(albedo.rgb, 1) * mix(0.1, 1.0, ambient) * vec4(kD, 1);
    FragColor += vec4(spec, 1);
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

