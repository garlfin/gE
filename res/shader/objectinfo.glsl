layout(std140, binding = 1) uniform ObjectsInfo
{
    mat4 Model[100];
    mat4 PreviousModel[100];
    mat4 NormalMatrix[100];
    uint ObjectCount;
};
