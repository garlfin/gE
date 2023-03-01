#ifndef INCLUDE_SSAO
#define INCLUDE_SSAO 1
#ifndef FORWARD

#ifndef SSAO_SAMPLES
#define SSAO_SAMPLES 8
#endif

#ifndef SSAO_RADIUS
#define SSAO_RADIUS 0.5
#endif

#ifndef SSAO_THICKNESS
#define SSAO_THICKNESS 0.5
#endif


float CalculateSSAO(vec3 normal)
{
    float ssaoSample = 0;

    for(int i = 0; i < SSAO_SAMPLES; i++)
    {
        vec3 hemiSample = vec3(interleavedGradientSample);
        hemiSample.xy = vogelDiskSample(i, SSAO_SAMPLES, interleavedGradientSample * PI2);
        hemiSample.xy = normalize(mix(vec2(0.1), vec2(1), abs(hemiSample.xy)) * sign(hemiSample.xy));
        hemiSample = normalize(hemiSample);
        hemiSample *= mix(.1, 1.0, interleavedGradientSample * interleavedGradientSample);
        hemiSample = FragPos + TBN * hemiSample * SSAO_RADIUS;

        vec3 hemiScreen = _worldToScreen(hemiSample);
        float delta = hemiScreen.z - _linearizeDepth(texture(FrameDepthTex, hemiScreen.xy).r, Info.zw);

        bool outOfBounds = any(greaterThan(hemiScreen.xy, vec2(1))) || any(lessThan(hemiScreen.xy, vec2(0)));
        ssaoSample += delta > 0 && delta < SSAO_THICKNESS + SSAO_RADIUS ? (outOfBounds ? 1 : 0) : 1;
    }

    ssaoSample /= SSAO_SAMPLES;
    return pow(ssaoSample, 2);
}


#endif
#endif