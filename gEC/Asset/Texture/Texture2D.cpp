//
// Created by scion on 11/3/2022.
//

#include <GLAD/glad.h>
#include "Texture2D.h"
#include "Texture.h"

namespace gE
{
    namespace Asset
    {
        void Texture2D::Use()
        {
            glBindTexture(GL_TEXTURE_2D, ID);
        }

        Texture2D::Texture2D(Window* window, uint32_t width, uint32_t height, TextureType type, uint8_t mipCount, TextureFilterMode filter, TextureWrapMode wrapMode)
        : Texture(window, width, height, type, mipCount)
        {
            if(!mipCount) CalculateMipCount();
            glCreateTextures(GL_TEXTURE_2D, 1, &ID);
            glTextureStorage2D(ID, MipMapCount, FormatToSizedFormat(type), width, height);

            glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST + (filter == TextureFilterMode::LINEAR) + (MipMapCount > 1) * 0x100);
            glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST + (filter == TextureFilterMode::LINEAR));
            glTextureParameteri(ID, GL_TEXTURE_WRAP_S, (GLenum) wrapMode);
            glTextureParameteri(ID, GL_TEXTURE_WRAP_T, (GLenum) wrapMode);
        }

        Texture2D::Texture2D(Window* const window, uint32_t width, uint32_t height, TextureFilterMode filter, TextureType type, uint8_t* data, uint8_t mipCount, uint8_t sentMips) : Texture2D(window, width, height, type, mipCount, filter)
        {
            bool compressed = IS_COMPRESSED(type);
            for(uint8_t i = 0; i < sentMips; i++)
            {
                auto mipSize = GetSize(i);

                if(compressed)
                    glCompressedTextureSubImage2D(ID, i, 0, 0, mipSize.x, mipSize.y, FormatToPixelFormat(type),
                                                  FormatToCompressionRatio(type).CalculateBytes(mipSize), data);
                else
                    glTextureSubImage2D(ID, i, 0, 0, mipSize.x, mipSize.y, FormatToPixelFormat(type), FormatToPixelType(Format), data);

                data += FormatToCompressionRatio(type).CalculateBytes(mipSize);
            }

            if(sentMips < MipMapCount) glGenerateTextureMipmap(ID);
        }
    }
}