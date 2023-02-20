layout(location = 0) in vec3 vPos;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(vPos, 1.0);
    TexCoord = vPos.xy * 0.5 + 0.5;
}