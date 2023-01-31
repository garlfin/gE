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

#include "../res/shdrinc/noise.glsl"
#include "../res/shdrinc/ray.glsl"
#include "../res/shdrinc/shadow.glsl"

out vec4 FragColor;

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 light = normalize(SunInfo.xyz);
    const vec3 incoming = normalize(FragPos - Position);

    vec3 rayPos = FragPos + light * 0.02 * (1 + interleavedGradientSample) + 0.01 * normal;

    float ambient = max(dot(normal, light), 0);
    ambient = min(ambient, CalculateShadow());

    FragColor = texture(sampler2D(Albedo), TexCoord) * mix(0.3, 1.0, ambient);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}
