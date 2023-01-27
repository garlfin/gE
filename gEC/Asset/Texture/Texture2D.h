//
// Created by scion on 11/3/2022.
//

#pragma once

#include "Texture.h"

namespace gE::Asset
{
    class Texture2D final : public Texture
    {
    public:
        Texture2D(Window* const window, uint32_t width, uint32_t height, TextureFilterMode filter, TextureType type,
                  uint8_t* data, uint8_t mipCount = 1, uint8_t sentMips = 1);

        Texture2D(Window* const window, uint32_t width, uint32_t height, TextureType type, uint8_t mipCount = 1,
                  TextureFilterMode filter = TextureFilterMode::LINEAR, TextureWrapMode wrapMode = TextureWrapMode::REPEAT);

        void Use() override;
    };
} // GLAsset
