//
// Created by scion on 11/7/2022.
//

#include "Shader.h"
#include "ShaderStage.h"
#include <GLAD/glad.h>

gE::Asset::Shader::Shader(gE::Window* window, const char* const vPath, const char* const fPath, CullMode cullMode, DepthFunction depthFunc) : Shader(window, cullMode, depthFunc)
{
    ShaderStage vertex(window, vPath, StageType::Vertex);
    ShaderStage fragment(window, fPath, StageType::Fragment);

    glAttachShader(ID, vertex.Get());
    glAttachShader(ID, fragment.Get());
    glLinkProgram(ID);
}

gE::Asset::Shader::~Shader()
{
    glDeleteProgram(ID);
}

void gE::Asset::Shader::Use()
{
    if((uint32_t) p_CullMode)
    {
        glEnable(GL_CULL_FACE);
        glCullFace((GLenum) p_CullMode);
    }
    else
        glDisable(GL_CULL_FACE);

    glDepthFunc((GLenum) p_DepthFunc);

    glUseProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window* window, const char* const path) : Shader(window, CullMode::NEVER, DepthFunction::ALWAYS)
{
    glAttachShader(ID, ShaderStage(window, path, StageType::Compute).Get());
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
