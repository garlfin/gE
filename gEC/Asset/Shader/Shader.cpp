//
// Created by scion on 11/7/2022.
//

#include "Shader.h"
#include "ShaderStage.h"
#include <GLAD/glad.h>

gE::Asset::Shader::Shader(gE::Window* window, const char* const vPath, const char* const fPath) : GLAsset(window)
{
    ID = glCreateProgram();

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
    glUseProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window* window, const char* const path) : GLAsset(window)
{
    ID = glCreateProgram();
    glAttachShader(ID, ShaderStage(window, path, StageType::Compute).Get());
    glLinkProgram(ID);
}

gE::Asset::Shader::Shader(gE::Window* window, gE::Asset::ShaderStage* v,
                          gE::Asset::ShaderStage* f) : GLAsset(window)
{
    ID = glCreateProgram();

    glAttachShader(ID, v->Get());
    glAttachShader(ID, f->Get());

    glLinkProgram(ID);
}
