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

vec4 SampleCubemap(CubemapData cm, vec3 dir, float roughness)
{
    vec3 boxMin = cm.Position - cm.Extents;
    vec3 boxMax = boxMin + cm.Extents * 2;

    vec3 firstIntersect = (boxMax - FragPos) / dir;
    vec3 secondIntersect = (boxMin -  FragPos) / dir;
    vec3 farIntersect = max(firstIntersect, secondIntersect);
    float distance = min(min(farIntersect.x, farIntersect.y), farIntersect.z);
    vec3 intersectPos = FragPos + dir * distance;
    vec3 sampleDir = intersectPos - cm.Position;

    if (any(greaterThan(FragPos, boxMax)) || any(lessThan(FragPos, boxMin)) || any(equal(cm.Extents, vec3(0)))) sampleDir = dir;

    return textureLod(SkyboxTex, sampleDir, textureQueryLevels(SkyboxTex) * roughness);
}

#endif
