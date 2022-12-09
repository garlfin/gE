//
// Created by scion on 11/7/2022.
//

#include <GLAD/glad.h>
#include <filesystem>
#include <iostream>
#include "ShaderStage.h"
#include "../../Windowing/Window.h"

gE::Asset::ShaderStage::ShaderStage(gE::Window* window, const char* const path, gE::Asset::StageType type) : GLAsset(window)
{
    if (!std::filesystem::exists(path)) throw std::runtime_error("Could not find requested file!");

    const char* shaderSource = window->IncludeManager.LoadIncludesRecurse(path);
    ID = glCreateShader((GLenum) type);
    glShaderSource(ID, 1, &shaderSource, nullptr);
    glCompileShader(ID);

    GLint length;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &length); // cheese

    if(length) return;
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length); // cheese: the reckoning

    char* log = new char[length + 1]{};
    glGetShaderInfoLog(ID, length, nullptr, log);
    std::cout << log << std::endl;
}

gE::Asset::ShaderStage::~ShaderStage()
{
    glDeleteShader(ID);
}

