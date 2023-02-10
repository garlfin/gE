out vec4 FragColor;

void main()
{
    vec2 c = floor(gl_FragCoord.xy / 20);
    float b =  mod(c.x + mod(c.y, 2.0), 2.0) * 0.97;
    FragColor = vec4(b, 0, b, 1);
}