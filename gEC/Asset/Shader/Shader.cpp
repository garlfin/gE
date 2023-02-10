//
// Created by scion on 11/7/2022.
//

#include "Shader.h"
#include "ShaderStage.h"
#include <GLAD/glad.h>
#include "../../Windowing/Window.h"

gE::Asset::Shader::Shader(gE::Window* window, const char* const vPath, const char* const fPath, CullMode cullMode, DepthFunction depthFunc, CompileFlags flags) : Shader(window, cullMode, depthFunc)
{
    ShaderStage vertex(window, vPath, StageType::Vertex, flags);
    ShaderStage fragment(window, fPath, StageType::Fragment, flags);

    glAttachShader(ID, vertex.Get());
    glAttachShader(ID, fragment.Get());
    glLinkProgram(ID);
}

gE::Asset::Shader::~Shader()
{
    glDeleteProgram(ID);
}

void gE::Asset::Shader::Use(gE::Asset::DepthFunction dOverride, CullMode cOverride) const
{
    if((uint32_t) p_CullMode || (uint32_t) cOverride)
    {
        glEnable(GL_CULL_FACE);
        glCullFace((uint32_t) cOverride ?: (uint32_t) p_CullMode);
    }
    else
        glDisable(GL_CULL_FACE);

    if(GetWindow()->GetStage() & (Windowing::Stage::PreZ | Windowing::Stage::Shadow | Windowing::Stage::PostProcess)) glDepthFunc((uint32_t) dOverride ?: (uint32_t) p_DepthFunc);
    if(GetWindow()->GetStage() == Windowing::Stage::Shadow) glDisable(GL_CULL_FACE);

    glUseProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window* window, const char* const path) : Shader(window, CullMode::NEVER, DepthFunction::ALWAYS)
{
    glAttachShader(ID, ShaderStage(window, path, StageType::Compute, CompileFlags::NONE).Get());
    glLinkProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window* window, gE::Asset::ShaderStage* v,
                          gE::Asset::ShaderStage* f, CullMode cullMode, DepthFunction depthFunc) : Shader(window, cullMode, depthFunc)
{
    glAttachShader(ID, v->Get());
    glAttachShader(ID, f->Get());

    glLinkProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window *window, gE::Asset::CullMode cullMode, gE::Asset::DepthFunction function) : GLAsset(window), p_CullMode(cullMode), p_DepthFunc(function)
{
    ID = glCreateProgram();
}

