#ifndef NOISE_INCLUDE
#define NOISE_INCLUDE 1

float interleavedGradientNoise(vec2 pos)
{
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
    return fract(magic.z * fract(dot(pos, magic.xy)));
}

float interleavedGradientNoise()
{
    return interleavedGradientNoise(gl_FragCoord.xy);
}

vec2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float r = sqrt(float(sampleIndex) + 0.5f) / sqrt(float(samplesCount));
    float theta = sampleIndex * 2.4 + phi;

    return vec2(r * cos(theta), r * sin(theta));
}

float interleavedGradientSample = interleavedGradientNoise();

#endif