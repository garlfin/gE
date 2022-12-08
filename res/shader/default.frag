#version 460 core

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
    vec3 normal = normalize(Normal);
    float light = dot(normal, normalize(vec3(1)));
    light = light * 0.5 + 0.5;
    light *= light;
    FragColor = vec4(light.rrr, 1);
}
