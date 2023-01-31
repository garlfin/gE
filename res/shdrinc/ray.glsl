#ifndef INCLUDE_RAY
#define INCLUDE_RAY 1

#define RAY_MODE_CHEAP 0
#define RAY_MODE_ACCURATE 1

#ifndef RAY_THICKNESS
#define RAY_THICKNESS 0.2
#endif

#ifndef RAY_THRESHOLD
#define RAY_THRESHOLD 0.05
#endif

// "Public" functions
vec2 CastRay(inout vec3 rayPos, vec3 rayDir, uint iteration, float length, uint rayMode);

// "Private" functions
vec3 _worldToScreen(vec3);
vec3 _worldToView(vec3);
vec3 _screenToWorld(vec3, bool);
vec2 _binaryRefine(inout vec3 rayPos, vec3 rayDir, uint iteration);
float _linearizeDepth(float z, vec2 p);

vec2 CastRay(inout vec3 rayPos, vec3 rayDir, uint iteration, float length, uint rayMode)
{
    rayDir = normalize(rayDir);
    rayDir *= length / iteration;

    for(uint i = 0; i < iteration; i++)
    {
        vec3 rayScreen = _worldToScreen(rayPos);
        float rayDepth = _linearizeDepth(textureLod(FrameDepthTex, rayScreen.xy, 0).r, Info.zw);
        float delta = rayMode == RAY_MODE_CHEAP ? rayDepth - rayScreen.z : rayScreen.z - rayDepth;

        if(abs(delta) <= RAY_THRESHOLD) return rayScreen.xy;
        if(rayScreen.x < 0 || rayScreen.x > 1 || rayScreen.y < 0 || rayScreen.y > 1) break;
        if(delta > 0 && delta <= RAY_THICKNESS)
            if(rayMode == RAY_MODE_CHEAP)
                return rayScreen.xy;
            else
                return _binaryRefine(rayPos, rayDir, iteration);

        rayPos += rayDir;
    }

    return vec2(-1);
}

vec2 _binaryRefine(inout vec3 rayPos, vec3 rayDir, uint iteration)
{
    for(uint i = 0; i < iteration; i++)
    {
        rayDir *= 0.5;
        rayPos += rayDir * 0.5;
        vec3 rayScreen = _worldToScreen(rayPos);
        float rayDepth = _linearizeDepth(textureLod(FrameDepthTex, rayScreen.xy, 0).r, Info.zw);

        if(abs(rayDepth - rayScreen.z) <= RAY_THRESHOLD) return rayScreen.xy;
        if(rayScreen.z < rayDepth) rayPos += rayDir * 0.5;
        else rayPos -= rayDir * 0.5;
    }
    return vec2(-1);
}

vec3 _worldToScreen(vec3 pos) { vec4 val = Projection * View * vec4(pos, 1.0); val.xy /= val.w; val.xy = val.xy * 0.5 + 0.5; return val.xyz; };
vec3 _worldToView(vec3 pos) { return (View * vec4(pos, 1.0)).xyz; }
vec3 _screenToWorld(vec3 pos, bool ndc)
{
    if(!ndc) pos = pos * 2 - 1;

    vec4 val = inverse(Projection) * vec4(pos, 1.0);
    val *= val.w;
    return (View * val).xyz;
}
float _linearizeDepth(float z, vec2 p) { return 2.0 * p.x * p.y / (p.y + p.x - (z * 2 - 1) * (p.y - p.x));}
float _linearizeDepthNDC(float z, vec2 p) { return p.x * p.y / (p.y + p.x - z * (p.y - p.x));}
#endif