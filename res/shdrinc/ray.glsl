#ifndef INCLUDE_RAY
#define INCLUDE_RAY 1

#define RAY_MODE_CHEAP 0
#define RAY_MODE_ACCURATE 1

#ifndef RAY_THICKNESS
#define RAY_THICKNESS 0.1
#endif

#ifndef RAY_REFINE
#define RAY_REFINE 40
#endif

#ifndef FORWARD
vec2 CastRay(inout vec3 rayPos, vec3 rayDir, uint iteration, float length, uint rayMode, float threshold);
vec2 _binaryRefine(inout vec3 rayPos, vec3 rayDir, float threshold);
#endif

vec3 _worldToScreen(vec3);
vec3 _worldToView(vec3);
vec3 _screenToWorld(vec3, bool);
float _linearizeDepth(float z, vec2 p);

#ifndef FORWARD
vec2 CastRay(inout vec3 rayPos, vec3 rayDir, uint iteration, float length, uint rayMode, float threshold)
{
    rayDir = normalize(rayDir);
    rayDir *= length / iteration;

    for(uint i = 0; i < iteration; i++)
    {
        rayPos += rayDir;
        vec3 rayScreen = _worldToScreen(rayPos);
        float rayDepth = _linearizeDepth(textureLod(FrameDepthTex, rayScreen.xy, 0).r, Info.zw);
        float delta = rayMode == RAY_MODE_CHEAP ? rayDepth - rayScreen.z : rayScreen.z - rayDepth;

        if(rayScreen.x < 0 || rayScreen.x > 1 || rayScreen.y < 0 || rayScreen.y > 1 || rayScreen.z < 0) break;

        if (delta > 0 && delta <= RAY_THICKNESS)
            if (rayMode == RAY_MODE_CHEAP)
                return rayScreen.xy;
            else
                return _binaryRefine(rayPos, rayDir, threshold);
    }

    return vec2(-1);
}

vec2 _binaryRefine(inout vec3 rayPos, vec3 rayDir, float threshold)
{
    vec3 rayScreen;
    rayPos -= rayDir;
    for(uint i = 0; i < RAY_REFINE; i++)
    {
        rayDir *= 0.5;
        rayPos += rayDir;
        rayScreen = _worldToScreen(rayPos);
        float rayDepth = _linearizeDepth(textureLod(FrameDepthTex, rayScreen.xy, 0).r, Info.zw);

        if(abs(rayScreen.z - rayDepth) <= threshold) return rayScreen.xy;
        if(rayScreen.z > rayDepth) rayPos -= rayDir;
    }
    return vec2(-1);
}
#endif

vec3 _worldToScreen(vec3 pos) { vec4 val = Projection * View * vec4(pos, 1.0); val.xy /= val.w; val.xy = val.xy * 0.5 + 0.5; return val.xyz; };
vec3 _worldToScreenNoJitter(vec3 pos) { vec4 val = Projection * View * vec4(pos, 1.0); val.xy /= val.w; val.xy = val.xy * 0.5 + 0.5; return val.xyz; };
vec3 _worldToScreenPrev(vec3 pos) { vec4 val = Projection * PreviousView * vec4(pos, 1.0); val.xy /= val.w; val.xy = val.xy * 0.5 + 0.5; return val.xyz; };
vec3 _worldToView(vec3 pos) { return (View * vec4(pos, 1.0)).xyz; }
vec3 _screenToWorld(vec3 pos, bool ndc)
{
    pos = ndc ? pos : pos * 2 - 1;

    vec4 val = inverse(Projection) * vec4(pos, 1.0);
    val /= val.w;
    return (inverse(View) * val).xyz;
}
float _linearizeDepth(float z, vec2 p) { return 2 * p.x * p.y / (p.y + p.x - (z * 2 - 1) * (p.y - p.x));}
#endif