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

out vec4 FragColor;

float interleavedGradientNoise()
{
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
    return fract(magic.z * fract(dot(gl_FragCoord.xy, magic.xy)));
}

vec2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{

    float r = sqrt(float(sampleIndex) + 0.5f) / sqrt(float(samplesCount));
    float theta = sampleIndex * 2.4 + phi;

    return vec2(r * cos(theta), r * sin(theta));
}

#define SAMPLE_COUNT 64

float calcShadow()
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    //float shadow = 0;
    //for(int i = 0; i < SAMPLE_COUNT; i++)
        return texture(ShadowTex, vec3(projCoords.xy, projCoords.z - 0.005));

    // return shadow / SAMPLE_COUNT;
}

void main()
{
    vec3 normal = normalize(Normal);

    float shadow = calcShadow();

    float light = dot(normal, SunInfo.xyz) * 0.5 + 0.5;
    light = min(min(shadow + 0.5, 1), light);
    light *= light;

    vec3 spec = pow(max(dot(reflect(-SunInfo.xyz, normal), normalize(Position - FragPos)), 0.0), 256.0).rrr;
    spec = mix(vec3(0), spec, shadow);

    FragColor = vec4(texture(sampler2D(Albedo), TexCoord).rgb * light + spec, 1);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}
