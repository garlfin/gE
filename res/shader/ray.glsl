#define RAY_THRESHOLD 0.005
#define RAY_OBJ_THICKNESS 0.2

float linearizeDepth(float z, vec2 planes);
vec3 worldToScreen(vec3 pos);
vec2 binaryRefine(inout vec3 rayPos, vec3 dir, uint steps);
vec3 screenToWorld(vec3 pos, bool ndc);

vec2 castRay(inout vec3 rayPos, vec3 dir, float maxLen, uint steps)
{
    dir = normalize(dir);
    dir *= maxLen / steps;
    //rayPos += dir;

    for(uint i = 0; i < steps; i++)
    {
        const vec3 screenPos = worldToScreen(rayPos);
        const float screenDepth = linearizeDepth(textureLod(FrameDepthTex, screenPos.xy, 0).r, Info.zw);
        const float delta = screenPos.z - screenDepth;

        if(screenPos.z < 0 || max(screenPos.x, screenPos.y) > 1 || min(screenPos.x, screenPos.y) < 0) return vec2(-1.0); // Early exit

        if(abs(delta) <= RAY_THRESHOLD) return screenPos.xy;
        if(delta > 0 && delta < RAY_OBJ_THICKNESS) return binaryRefine(rayPos, dir, steps);

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
    return 2.0 * planes.x * planes.y / (planes.y + planes.x - (z * 2 - 1) * (planes.y - planes.x));
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

vec3 randCone(vec3 incoming, float radius)
{
    vec3 h = vec3(interleavedGradientNoise(), max(interleavedGradientNoise(), 0.1), interleavedGradientNoise());
    h.xz = h.xy * 2 - 1;
    h = normalize(h);
    h.xz *= radius;

    return reflect(incoming, normalize(h));
}