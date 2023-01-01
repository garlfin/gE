layout(std140, binding = 0) uniform SceneInfo
{
    mat4 View;
    mat4 Projection;
    vec4 Info; // Aspect, FOV, Near, Far
    vec3 Position;
};


/** TODO implement cubemap
mat4 GetView(out CameraCubemap camera, uint side)
{
    if (bool(side))
    return View[side - 1];
    else
    return Base.View;
}*/
