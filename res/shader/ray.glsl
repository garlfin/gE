#define RAY_THRESHOLD 0.1
#define RAY_OBJ_THICKNESS 1.0
#define RAY_REFINE 50

float linearizeDepth(float z, vec2 planes);
vec3 worldToScreen(vec3 pos);
vec2 binaryRefine(inout vec3 rayPos, vec3 dir, uint steps);
vec3 screenToWorld(vec3 pos, bool ndc);

vec2 castRay(inout vec3 rayPos, vec3 dir, float maxLen, uint steps)
{
    dir = normalize(dir);
    dir *= maxLen / steps;
    rayPos += dir;

    for(uint i = 0; i < steps; i++)
    {
        const vec3 screenPos = worldToScreen(rayPos);
        const float screenDepth = linearizeDepth(texture(FrameDepthTex, screenPos.xy).r, Info.zw);
        const float delta = screenPos.z - screenDepth;

        if(screenPos.z < 0 || max(screenPos.x, screenPos.y) > 1 || min(screenPos.x, screenPos.y) < 0) return vec2(-1.0); // Early exit
        if(abs(delta) <= RAY_THRESHOLD) return screenPos.xy;
        if(delta > 0 && delta < RAY_OBJ_THICKNESS) return binaryRefine(rayPos, dir, RAY_REFINE);

        rayPos += dir;
    }

    return vec2(-1.0);
}

vec2 binaryRefine(inout vec3 rayPos, vec3 dir, uint steps)
{
    vec3 screenPos;

    for(uint i = 0; i < steps; i++)
    {
        screenPos = worldToScreen(rayPos);
        const float screenDepth = linearizeDepth(texture(FrameDepthTex, screenPos.xy).r, Info.zw);
        const float delta = screenPos.z - screenDepth;

        if(abs(delta) <= RAY_THRESHOLD) return screenPos.xy;

        dir *= delta < 0 ? 0.5 : -0.5;
        rayPos += dir;
    }

    return vec2(-1.0);
}

float linearizeDepth(float z, vec2 planes)
{
    float z_n = 2.0 * z - 1.0;
    return 2.0 * planes.x * planes.y / (planes.y + planes.x - z_n * (planes.y - planes.x));
}

vec3 worldToScreen(vec3 pos)
{
    vec4 outVal = Projection * View * vec4(pos, 1.0);
    outVal.xy /= outVal.w;
    outVal.xy = outVal.xy * 0.5 + 0.5;
    return outVal.xyz;
}

vec3 screenToWorld(vec3 pos, bool ndc)
{
    if(ndc) pos = pos * 2 - 1;
    vec4 outVal = inverse(Projection) * vec4(pos, 1.0);
    outVal /= outVal.w;
    outVal = inverse(View) * outVal;
    return outVal.xyz;
}

float InterleavedGradientNoise(vec2 pos) {
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829341);
    return fract(magic.z * fract(dot(pos, magic.xy)));
}

float ditherSample = InterleavedGradientNoise(gl_FragCoord.xy);

vec3 hash(vec3 a)
{
    a = fract(a);
    a += dot(a, a.yxz + 19.19);
    return fract((a.xxy + a.yxx)*a.zyx);
}

vec3 randCone(float radius, inout mat3 tbn)
{
    vec3 h = normalize(hash(FragPos)) * ditherSample;
    h.xz *= radius;

    return normalize(tbn * normalize(h));
}