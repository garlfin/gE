layout(std140, binding = 0) uniform SceneInfo
{
    mat4 View;
    mat4 PreviousView;
    mat4 Projection;
    mat4 PreviousProjection;
    vec4 Info; // Width, Height, Near, Far
    vec3 CamPos;
#ifndef FORWARD
    sampler2D FrameColorTex;
    sampler2D FrameDepthTex;
#else
    vec4 _pad;
#endif
};

#define UP    vec4(0, 1, 0, 0)
#define LEFT  vec4(1, 0, 0, 0)
#define FRONT vec4(0, 0, 1, 0)

mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(up, f));
    vec3 u = cross(f, s);

    mat4 Result = mat4(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = f.x;
    Result[1][2] = f.y;
    Result[2][2] = f.z;
    Result[3][0] = -dot(s, eye);
    Result[3][1] = -dot(u, eye);
    Result[3][2] = -dot(f, eye);
    return Result;
}

mat4 GetView(vec3 pos, uint side)
{
    // X+, X-, Y+, Y-, Z+ Z-
    if(side == 1)
        return lookAt(pos, pos + vec3(1, 0, 0), vec3(0, -1, 0));
    if(side == 2)
        return lookAt(pos, pos + vec3(0, -1, 0), vec3(0, 0, 1));
    if(side == 3)
        return lookAt(pos, pos + vec3(0, 1, 0), vec3(0, 0, -1));
    if(side == 4)
        return lookAt(pos, pos + vec3(0, 0, -1), vec3(0, -1, 0));
    if(side == 5)
        return lookAt(pos, pos + vec3(0, 0, 1), vec3(0, -1, 0));

    return View;
}


