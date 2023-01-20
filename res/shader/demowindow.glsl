layout(std140, binding = 2) uniform DemoWindow
{
    samplerCube SkyboxTex;
    uvec2 ShadowTex;
    vec4 SunInfo; // Direction, ShadowSize
    mat4 SunMatrix;
    sampler2D FrameColorTex;
    sampler2D FrameDepthTex;
};