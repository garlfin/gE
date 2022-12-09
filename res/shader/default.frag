#version 460 core
#extension GL_ARB_bindless_texture: enable
#include "../res/shader/camera.glsl"

layout(std140, binding = 0) uniform SceneInfo
{
    Camera cam;
};
uniform sampler2D Albedo;

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

    FragColor = vec4(texture(Albedo, TexCoord).rgb * light + spec, 1);
}
