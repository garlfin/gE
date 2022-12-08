//
// Created by scion on 11/15/2022.
//

#pragma once

#include "Texture.h"

namespace gE::Asset
{
    class TextureCube : public Texture
    {
    public:
        TextureCube(Window* const window, uint32_t size, TextureType type,
        uint8_t mipCount = 1);
        TextureCube(Window* const window, uint32_t size, TextureFilterMode filter, TextureType type,
        uint8_t* data, uint8_t mipCount = 1, uint8_t sentMips = 1);

        void Use() override;
    };
}
