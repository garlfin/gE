layout(std140, binding = 0) uniform SceneInfo
{
    mat4 View;
    mat4 Projection;
    vec4 Info; // Width, Height, Near, Far
    vec3 Position;
};

#define UP    vec4(0, 1, 0, 0)
#define LEFT  vec4(1, 0, 0, 0)
#define FRONT vec4(0, 0, 1, 0)

mat4 GetView(vec3 pos, uint side)
{
    // X+, X-, Y+, Y-, Z+ Z-

    if(side == 0)
        return View;
    if(side == 1)
        return mat4(-FRONT, UP, -LEFT, vec4(pos, 1));
    if(side == 2)
        return mat4(-LEFT, FRONT, UP, vec4(pos, 1));
    if(side == 3)
        return mat4(-LEFT, -FRONT, -UP, vec4(pos, 1));
    if(side == 4)
        return mat4(LEFT, UP, FRONT, vec4(pos, 1));
    if(side == 5)
        return mat4(-LEFT, UP, -FRONT, vec4(pos, 1));
}
