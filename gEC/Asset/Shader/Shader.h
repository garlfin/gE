//
// Created by scion on 11/7/2022.
//

#pragma once


#include "../GLAsset.h"
#include "ShaderStage.h"

namespace gE::Asset
{
    enum class CullMode : uint32_t
    {
        NEVER = 0,
        BACKFACE = 0x0405,
        FRONT_FACE = 0x0404
    };

    enum class DepthFunction : uint32_t
    {
        ALWAYS = 0x0207,
        LESS = 0x0201,
        LEQUAL = 0x0203
    };

    class Shader : public GLAsset
    {
    private:
        CullMode p_CullMode;
        DepthFunction p_DepthFunc;
        Shader(Window* window, CullMode cullMode, DepthFunction function);
    public:
        Shader(gE::Window* window, gE::Asset::ShaderStage*,
               gE::Asset::ShaderStage*, CullMode cullMode = CullMode::BACKFACE, DepthFunction depthFunc = DepthFunction::LESS);
        Shader(gE::Window* window, const char* const, const char* const, CullMode cullMode = CullMode::BACKFACE, DepthFunction depthFunc = DepthFunction::LESS);
        Shader(gE::Window* window, const char* const);
        ~Shader() override;

        void Use() const;
    };
}
