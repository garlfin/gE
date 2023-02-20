layout(location = 0) uniform sampler2D Tex;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
    FragColor = pow(texture(Tex, TexCoord), vec4(1.0 / 2.2));
}