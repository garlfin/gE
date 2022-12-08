//
// Created by scion on 11/7/2022.
//

#pragma once


#include "../GLAsset.h"
#include "ShaderStage.h"

namespace gE::Asset {
    class Shader : public GLAsset
    {
    public:
        Shader(gE::Window* window, gE::Asset::ShaderStage*,
               gE::Asset::ShaderStage*);
        Shader(gE::Window* window, const char* const, const char* const);
        Shader(gE::Window* window, const char* const);
        ~Shader() override;

        void Use();
    };
}
