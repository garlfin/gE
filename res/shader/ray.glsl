#ifndef RAY_INCLUDE
#define RAY_INCLUDE 1

#include "../res/shader/noise.glsl"
#define RAY_THRESHOLD 0.005
#define RAY_OBJ_THICKNESS 0.2

float linearizeDepth(float z, vec2 planes);
vec3 worldToScreen(vec3 pos);
vec2 binaryRefine(inout vec3 rayPos, vec3 dir, uint steps);
vec3 screenToWorld(vec3 pos, bool ndc);

#define RAY_MODE_EXPENSIVE 0
#define RAY_MODE_CHEAP 1

vec2 castRay(inout vec3 rayPos, vec3 dir, float maxLen, uint steps, int mode)
{
    dir = normalize(dir);
    dir *= maxLen / steps;

    for(uint i = 0; i < steps; i++)
    {
        const vec3 screenPos = worldToScreen(rayPos);
        if(screenPos.z < 0 || max(screenPos.x, screenPos.y) > 1 || min(screenPos.x, screenPos.y) < 0) break; // Early exit
        const float screenDepth = linearizeDepth(textureLod(FrameDepthTex, screenPos.xy, 0).r, Info.zw);
        const float delta = screenPos.z - screenDepth;

        if (abs(delta) <= RAY_THRESHOLD || (mode == RAY_MODE_CHEAP && delta < 0 && delta > -RAY_OBJ_THICKNESS)) return screenPos.xy;
        if (delta > 0 && delta < RAY_OBJ_THICKNESS) return binaryRefine(rayPos, dir, steps);
        if (mode == RAY_MODE_CHEAP && delta > 0) return vec2(1.0);

        rayPos += dir;
    }

    return vec2(-1.0);
}

vec2 binaryRefine(inout vec3 rayPos, vec3 dir, uint steps)
{

    vec3 minRaySample = rayPos - dir;
    vec3 maxRaySample = rayPos;
    vec3 rayScreenPos, midRaySample;

    for(uint i = 0; i < steps; i++)
    {
        midRaySample = (minRaySample + maxRaySample) / 2;
        rayScreenPos = worldToScreen(midRaySample);
        const float screenDepth = linearizeDepth(textureLod(FrameDepthTex, rayScreenPos.xy, 0).r, Info.zw);
        const float delta = screenDepth - rayScreenPos.z;

        if(rayScreenPos.z > screenDepth)
             maxRaySample = midRaySample;
        else
            minRaySample = midRaySample;

        if(abs(delta) <= RAY_THRESHOLD) return rayScreenPos.xy;
    }

    return vec2(-1.0);
}

float linearizeDepth(float z, vec2 planes)
{
    return 2.0 * planes.x * planes.y / (planes.y + (z * 2.0 - 1.0) * (planes.x - planes.y));
}

vec3 worldToScreen(vec3 pos)
{
    vec4 outVal = Projection * View * vec4(pos, 1.0);
    outVal.xy /= outVal.w;
    outVal.xy = outVal.xy * 0.5 + 0.5;
    return outVal.xyz;
}

vec3 worldToScreenNDC(vec3 pos)
{
    vec4 outVal = Projection * View * vec4(pos, 1.0);
    outVal.xyz /= outVal.w;
    outVal.xy = outVal.xy * 0.5 + 0.5;
    //outVal.z = outVal.z * 2 - 1;
    return outVal.xyz;
}

vec3 worldToView(vec3 pos)
{
    return (View * vec4(pos, 1.0)).xyz;
}

vec3 screenToWorld(vec3 pos, bool ndc)
{
    if(ndc) pos = pos * 2 - 1;
    vec4 outVal = inverse(Projection) * vec4(pos, 1.0);
    outVal /= outVal.w;
    outVal = inverse(View) * outVal;
    return outVal.xyz;
}

#endif
