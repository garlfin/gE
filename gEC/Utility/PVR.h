//
// Created by scion on 11/3/2022.
//

#pragma once

#include "../Asset/Texture/Texture.h"

namespace gE::Utility
{
    enum class TextureReturnType
    {
        Texture2D,
        TextureCubeMap
    };

    [[nodiscard]] gE::Asset::Texture* LoadPVR(Window* window, const char* const, TextureReturnType* returnType);
}
