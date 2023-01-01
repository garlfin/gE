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

#define ITERATIONS 200
#define MAX_LENGTH 10.0
#define ROUGHNESS 0.3

void main()
{
    const vec3 normal = normalize(Normal);
    const vec3 viewDir = normalize(FragPos - Position);

    const vec3 rayDir = randCone(reflect(viewDir, normal), ROUGHNESS);
    const vec2 screenUV = gl_FragCoord.xy / vec2(1280, 720);

    vec3 rayPos = screenToWorld(vec3(screenUV, texture(FrameDepthTex, screenUV).r), true);
    vec2 reflection = castRay(rayPos, rayDir, MAX_LENGTH, ITERATIONS);

    if(reflection.x == -1.0)
        FragColor = pow(texture(SkyboxTex, -rayDir), vec4(1.0 / 2.2));
    else
        FragColor = texture(FrameColorTex, reflection);
}


