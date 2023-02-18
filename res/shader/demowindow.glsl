layout(std140, binding = 2) uniform DemoWindow
{
    uvec2 ShadowTex;
    vec2 _pad_demoWindow;
    vec4 SunInfo; // Direction, ShadowSize
    mat4 SunMatrix;
    sampler2D BRDFLut;
    int Frame;
};