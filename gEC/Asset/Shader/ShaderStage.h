//
// Created by scion on 11/7/2022.
//

#pragma once

#include "../GLAsset.h"

namespace gE::Asset
{
    enum class StageType : uint32_t
    {
        Fragment = 0x8B30,
        Vertex = 0x8B31,
        Compute = 0x91B9
    };

    class ShaderStage : public GLAsset
    {
    public:
        ShaderStage(gE::Window* window, const char* const, gE::Asset::StageType);
        ~ShaderStage() override;
    };
}
