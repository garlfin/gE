#extension GL_ARB_bindless_texture : enable
layout(location = 0) uniform sampler2D NewTex;
layout(location = 1) uniform sampler2D OldTex;
layout(location = 2) uniform sampler2D VelocityTex;

#include "../res/shader/camera.glsl"

in vec2 TexCoord;
out vec4 FragColor;

#define HALTON_SAMPLES 16

void main()
{
    vec2 previousTexCoord = TexCoord - texture(VelocityTex, TexCoord).xy;

    vec4 currentColor = texture(NewTex, TexCoord);
    vec4 prevColor = texture(OldTex, TexCoord);

    vec4 nearColors[4] =
    {
    texture(NewTex, TexCoord + vec2(1, 0) / Info.xy),
    texture(NewTex, TexCoord + vec2(0, 1) / Info.xy),
    texture(NewTex, TexCoord + vec2(-1, 0) / Info.xy),
    texture(NewTex, TexCoord + vec2(0, -1) / Info.xy)
    };

    vec4 boxMin = min(currentColor, min(nearColors[0], min(nearColors[1], min(nearColors[2], nearColors[3]))));
    vec4 boxMax = max(currentColor, max(nearColors[0], max(nearColors[1], max(nearColors[2], nearColors[3]))));

    FragColor = mix(clamp(prevColor, boxMin, boxMax), texture(NewTex, TexCoord), 1.0 / HALTON_SAMPLES);
}