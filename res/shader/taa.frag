#extension GL_ARB_bindless_texture : enable
layout(location = 0) uniform sampler2D NewTex;
layout(location = 1) uniform sampler2D OldTex;
layout(location = 2) uniform sampler2D VelocityTex;

#include "../res/shader/camera.glsl"

in vec2 TexCoord;
out vec4 FragColor;

#define HALTON_SAMPLES 16
#define AVG_PLUS

void main()
{
    vec2 previousTexCoord = TexCoord - texture(VelocityTex, TexCoord).xy;

    vec4 currentColor = texture(NewTex, TexCoord);
    vec4 prevColor = texture(OldTex, previousTexCoord);

    vec2 oneOverSize = 1 / Info.xy;
    vec4 nearColors
        [
#ifdef AVG_PLUS
            8
#else
            4
#endif
        ] =
    {
    texture(NewTex, TexCoord + vec2(1, 0)   * oneOverSize),
    texture(NewTex, TexCoord + vec2(0, 1)   * oneOverSize),
    texture(NewTex, TexCoord + vec2(-1, 0)  * oneOverSize),
    texture(NewTex, TexCoord + vec2(0, -1)  * oneOverSize),
            #ifdef AVG_PLUS
    texture(NewTex, TexCoord + vec2(-1, -1) * oneOverSize),
    texture(NewTex, TexCoord + vec2(1, -1)  * oneOverSize),
    texture(NewTex, TexCoord + vec2(-1, 1)  * oneOverSize),
    texture(NewTex, TexCoord + vec2(1, 1)   * oneOverSize)
            #endif
    };

    vec4 boxMinPlus = min(currentColor, min(nearColors[0], min(nearColors[1], min(nearColors[2], nearColors[3]))));
    vec4 boxMaxPlus = max(currentColor, max(nearColors[0], max(nearColors[1], max(nearColors[2], nearColors[3]))));

#ifdef AVG_PLUS
    vec4 boxMin = currentColor;
    vec4 boxMax = currentColor;
    #pragma unroll
    for(int i = 0; i < 8; i++) boxMin = min(boxMin, nearColors[i]);
    #pragma unroll
    for(int i = 0; i < 8; i++) boxMax = max(boxMax, nearColors[i]);

    boxMinPlus = (boxMin + boxMinPlus) / 2;
    boxMaxPlus = (boxMax + boxMaxPlus) / 2;
#endif

    FragColor = mix(clamp(prevColor, boxMinPlus, boxMaxPlus), currentColor, any(greaterThan(previousTexCoord, vec2(1))) || any(lessThan(previousTexCoord, vec2(0))) ? 1 : 1.0 / HALTON_SAMPLES);
}