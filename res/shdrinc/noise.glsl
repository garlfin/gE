#ifndef NOISE_INCLUDE
#define NOISE_INCLUDE 1

#define PI2 6.28318

const mat4 _thresholdMatrix =
mat4(
1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0,
13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  7.0 / 17.0,
4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0,
16.0 / 17.0,  8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0
);

float interleavedGradientNoise(vec2 pos)
{
    const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
    return fract(magic.z * fract(dot(pos, magic.xy)));
}

vec2 vogelDiskSample(uint sampleIndex, int samplesCount, float phi)
{
    float r = sqrt(float(sampleIndex) + 0.5f) / sqrt(float(samplesCount));
    float theta = sampleIndex * 2.4 + phi;

    return vec2(r * cos(theta), r * sin(theta));
}

float dither(vec2 pos)
{
    return _thresholdMatrix[int(pos.x) % 4][int(pos.y) % 4];
}

#ifdef IGN_MOVING
float interleavedGradientSample = fract(interleavedGradientNoise(gl_FragCoord.xy) + Frame / 64.0);
#else
float interleavedGradientSample = interleavedGradientNoise(gl_FragCoord.xy);
#endif
float ditherSample = dither(gl_FragCoord.xy);
#endif