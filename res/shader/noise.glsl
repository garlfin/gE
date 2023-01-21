#ifndef NOISE_INCLUDE
#define NOISE_INCLUDE 1

float interleavedGradientNoise()
{
    float x = float(gl_FragCoord.x) + 5.588238f * (Frame % 64);
    float y = float(gl_FragCoord.y) + 5.588238f * (Frame % 64);
    return fract(52.9829189 * fract(0.06711056 * x + 0.00583715 * y));
}

float interleavedGradientNoise(vec2 pos)
{
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
    return fract(magic.z * fract(dot(pos * vec2(1280, 720), magic.xy)));
}

vec2 vogelDiskSample(int sampleIndex, int samplesCount, float phi)
{
    float r = sqrt(float(sampleIndex) + 0.5f) / sqrt(float(samplesCount));
    float theta = sampleIndex * 2.4 + phi;

    return vec2(r * cos(theta), r * sin(theta));
}

const float interleavedGradientSample = interleavedGradientNoise();

#endif