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

#define SHADOW_SAMPLES 8
#define SHADOW_BIAS 0.001
#define RAY_THICKNESS 0.2
#define METALLIC 0

#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"
#include "../res/shdrinc/shadow.glsl"

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

out vec4 FragColor;

#define ROUGHNESS 0.2

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 light = normalize(SunInfo.xyz);
    const vec3 incoming = normalize(FragPos - Position);
    const vec4 albedo = texture(sampler2D(Albedo), TexCoord);

    float ambient = max(dot(normal, light), 0);

    #ifndef FORWARD
    vec3 rayPos = FragPos + normal * 0.01 + SunInfo.xyz * (0.1 / 25) * (1 + interleavedGradientSample);
    ambient = min(ambient, CastRay(rayPos, SunInfo.xyz, 25, 0.1, RAY_MODE_CHEAP, 0.02).x == -1 ? 1 : 0);
    #endif


    const vec3 kS = fresnelSchlickRoughness(max(0, dot(normal, -incoming)), vec3(0.04), ROUGHNESS);
    vec2 brdf = texture(BRDFLut, vec2(clamp(dot(-incoming, normal), 0, 1), ROUGHNESS)).rg;
    vec3 spec = textureLod(SkyboxTex, reflect(incoming, normal), ROUGHNESS * textureQueryLevels(SkyboxTex)).rgb;
    spec *= kS * brdf.x + brdf.y;

    ambient = min(ambient, CalculateShadow(0.1));
    FragColor = albedo * mix(0.3, 1.0, ambient) + vec4(spec, 0);
}
