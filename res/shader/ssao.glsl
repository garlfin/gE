#ifndef SSAO
#define SSAO 1

#define SSAO_SAMPLE 32
#define SSAO_RADIUS 0.1
#define PI 3.14159


float ComputeSSAO()
{
    float avgSample = 0;
    float aoFragZ = worldToScreen(FragPos).z;

    for(int i = 0; i < SSAO_SAMPLE; i++)
    {
        vec3 vogelSample = vec3(vogelDiskSample(i, SSAO_SAMPLE, interleavedGradientSample * 2 * PI), 0);
        vogelSample.z = interleavedGradientNoise(vogelSample.xy * 1024);
        vec3 hemiSample = normalize(vogelSample) * interleavedGradientNoise(vogelSample.xz);

        hemiSample *= SSAO_RADIUS;
        hemiSample = TBN * hemiSample + FragPos;

        vec3 hemiScreen = worldToScreen(hemiSample);
        float linearDepth = linearizeDepth(textureLod(FrameDepthTex, hemiScreen.xy, 0).r, Info.zw);

        if (hemiScreen.z > linearDepth) avgSample += 1.0 * smoothstep(0, 1, SSAO_RADIUS / (aoFragZ - linearDepth));
    }

    return 1 - (avgSample / SSAO_SAMPLE);
}

#endif