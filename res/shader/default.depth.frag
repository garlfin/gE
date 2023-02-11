#extension GL_ARB_bindless_texture : enable

out vec4 FragColor;
in vec2 TexCoord;

uniform uvec2 Albedo;

#include "../res/shader/demowindow.glsl"
#include "../res/shader/camera.glsl"
#include "../res/shdrinc/noise.glsl"

void main()
{
    if(texture(sampler2D(Albedo), TexCoord).a <= ditherSample) discard;

    vec2 c = floor(gl_FragCoord.xy / 20);
    float b =  mod(c.x + mod(c.y, 2.0), 2.0) * 0.97;
    FragColor = vec4(b, 0, b, 1);
}