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
        glTextureStorage2D(ID, MipMapCount, FormatToPixelFormat(type), size, size);

        glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureParameteri(ID, GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_TRUE);
        glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    TextureCube::TextureCube(Window* const window, uint32_t size,
                             TextureType type, uint8_t* data, uint8_t mipCount, uint8_t sentMips) : TextureCube(window, size, type, mipCount)
    {
        bool compressed = IS_COMPRESSED(type);
        for(uint8_t mip = 0; mip < sentMips; mip++)
        {
            auto mipSize = GetSize(mip);

            for(uint8_t face = 0; face < 6; face++)
            {
                if(compressed)
                    glCompressedTextureSubImage3D(ID, mip, 0, 0, face, mipSize.x, mipSize.y, 1, FormatToPixelFormat(type),
                                                  FormatToCompressionRatio(type).CalculateBytes(mipSize), data);
                else
                    glTextureSubImage3D(ID, mip, 0, 0, face, mipSize.x, mipSize.y, 1, GL_RGB, GL_FLOAT, data);
                data += FormatToCompressionRatio(type).CalculateBytes(mipSize);
            }
        }

        if(sentMips < MipMapCount) glGenerateTextureMipmap(ID);
    }
}