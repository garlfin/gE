#version 460 core
#extension GL_ARB_bindless_texture: enable
#include "../res/shader/camera.glsl"

layout(early_fragment_tests) in;

layout(std140, binding = 0) uniform SceneInfo
{
    Camera cam;
};
uniform uvec2 Albedo;

in FragInfo
{
    vec3 FragPos;
    uvec4 InstanceInfo;
    vec3 Normal;
    vec2 TexCoord;
};

out vec4 FragColor;

void main()
{
    vec3 sun = normalize(vec3(1));
    vec3 normal = normalize(Normal);

    float light = dot(normal, sun) * 0.5 + 0.5;
    light *= light;

    vec3 spec = pow(max(dot(reflect(-sun, normal), normalize(cam.Position - FragPos)), 0.0), 256.0).rrr;

    FragColor = vec4(texture(sampler2D(Albedo), TexCoord).rgb * light + spec, 1);
    FragColor = pow(FragColor, vec4(1.0 / 2.2));
}
