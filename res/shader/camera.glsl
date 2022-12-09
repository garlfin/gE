struct Camera
{
    mat4 View;
    mat4 Projection;
    vec3 Position;
    vec4 Info; // Aspect, FOV, Near, Far
};

struct CameraCubemap
{
    Camera Base;
    mat4 View[5];
};

mat4 GetView(out CameraCubemap camera, uint side)
{
    if (bool(side))
        return camera.View[side - 1];
    else
        return camera.Base.View;
}