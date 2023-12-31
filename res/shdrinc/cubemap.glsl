#ifndef CUBEMAP_INCLUDE
#define CUBEMAP_INCLUDE 1

struct CubemapData
{
    samplerCube Cubemap;
    vec3 Position;
    vec3 Extents;
};

layout(std140, binding = 3) uniform CubemapManager
{
    samplerCube SkyboxTex;
    CubemapData Cubemaps[];
};

vec3 SampleAABB(CubemapData cm, vec3 dir);

vec4 SampleCubemap(CubemapData cm, vec3 dir)
{
    vec4 a = clamp(textureLod(cm.Cubemap, SampleAABB(cm, dir), 0), 0, 5);
    return clamp(mix(textureLod(SkyboxTex, dir, 0), a, a.a), 0, 5);
}

vec3 SampleAABB(CubemapData cm, vec3 dir)
{
    vec3 boxMin = cm.Position - cm.Extents;
    vec3 boxMax = boxMin + cm.Extents * 2;

    vec3 firstIntersect = (boxMax - FragPos) / dir;
    vec3 secondIntersect = (boxMin -  FragPos) / dir;
    vec3 farIntersect = max(firstIntersect, secondIntersect);
    float distance = min(min(farIntersect.x, farIntersect.y), farIntersect.z);
    if (any(greaterThan(FragPos, boxMax)) || any(lessThan(FragPos, boxMin)) || any(equal(cm.Extents, vec3(0)))) return dir;
    return (FragPos + dir * distance) - cm.Position;
}

#endif
