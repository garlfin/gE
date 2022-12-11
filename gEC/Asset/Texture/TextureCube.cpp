//
// Created by scion on 11/15/2022.
//

#include "TextureCube.h"
#include <GLAD/glad.h>
#include "Texture.h"

namespace gE::Asset
{
    void TextureCube::Use()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }

    TextureCube::TextureCube(Window* const window, uint32_t size,
                             TextureType type, uint8_t mipCount) : Texture(window, size, size, type, mipCount)
    {
        if(!mipCount) CalculateMipCount();
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &ID);
        glTextureStorage2D(ID, MipMapCount, FormatToSizedFormat(Format), size, size);

        glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    TextureCube::TextureCube(Window* const window, uint32_t size, TextureFilterMode filter,
                             TextureType type, uint8_t* data, uint8_t mipCount, uint8_t sentMips) : TextureCube(window, size, type, mipCount)
    {
        bool compressed = IS_COMPRESSED(type);
        for(uint8_t i = 0; i < sentMips; i++)
        {
            auto mipSize = GetSize(i);

            for(uint8_t f = 0; f < 6; f++)
            {
                if(compressed)
                    glCompressedTextureSubImage3D(ID, i, 0, 0, f, mipSize.x, mipSize.y, 1, FormatToPixelFormat(type),
                                                  FormatToCompressionRatio(type).CalculateBytes(mipSize), data);
                else
                    glTextureSubImage3D(ID, i, 0, 0, f, mipSize.x, mipSize.y, 1, GL_RGB, GL_FLOAT, data);
                data += FormatToCompressionRatio(type).CalculateBytes(mipSize);
            }
        }

        glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST + (filter == TextureFilterMode::LINEAR) + (MipMapCount > 1) * 0x100);
        glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST + (filter == TextureFilterMode::LINEAR));

        if(sentMips < MipMapCount) glGenerateTextureMipmap(ID);
    }
}