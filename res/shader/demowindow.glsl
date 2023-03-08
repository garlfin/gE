layout(std140, binding = 2) uniform DemoWindow
{
    uvec2 ShadowTex;
    uvec2 _pad_demoWindow;
    vec4 SunInfo; // Direction, ShadowSize
    mat4 SunMatrix;
    sampler2D BRDFLut;
    uvec2 _pad_sampler;
    int Frame;
    int Stage;
};

#define STAGE_UPDATE      1
#define STAGE_PREZ        2
#define STAGE_RENDER      4
#define STAGE_SHADOW      8
#define STAGE_POSTPROCESS 16
#define STAGE_CUBEMAP     32
#define STAGE_CUBEMAP_PREZ 64
