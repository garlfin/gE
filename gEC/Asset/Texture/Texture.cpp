//
// Created by scion on 11/3/2022.
//

#include "Texture.h"
#include "GLAD/glad.h"
#include <cmath>
#include <iostream>

namespace gE::Asset
{
    void Texture::CalculateMipCount()
    {
        MipMapCount = 1 + uint8_t(std::floor(std::log2(std::max(Width, Height))));
    }

    void Texture::CreateHandle()
    {
        if(Handle || !GLAD_GL_ARB_bindless_texture) return;

        Handle = glGetTextureHandleARB(ID);
        glMakeTextureHandleResidentARB(Handle);
    }

    Texture::~Texture()
    {
        if (Handle)
        {
            glMakeTextureHandleNonResidentARB(Handle);
            Handle = 0;
        }
        glDeleteTextures(1, &ID);
    }

    uint32_t Texture::Bind(uint32_t slot, AccessMode accessMode, uint8_t mip)
    {
        glBindImageTexture(slot, ID, mip, GL_FALSE, 0, (GLenum) accessMode, FormatToPixelFormat(Format));
        return slot;
    }

    uint32_t Texture::Use(uint32_t slot)
    {
        glBindTextureUnit(slot, ID);
        return slot;
    }

    void Texture::BindToFramebuffer(Framebuffer *framebuffer, GLenum location, uint32_t mip)
    {
        glNamedFramebufferTexture(framebuffer->Get(), location, ID, mip);
    }

    GLenum FormatToPixelFormat(gE::Asset::TextureType type)
    {
        switch(type)
        {
            case gE::Asset::TextureType::COMPRESSED_DX1:
                return GL_COMPRESSED_RGBA;
            case gE::Asset::TextureType::COMPRESSED_DX1_SRGB:
                return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
            case gE::Asset::TextureType::COMPRESSED_DX3:
                return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            case gE::Asset::TextureType::COMPRESSED_DX3_SRGB:
                return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
            case gE::Asset::TextureType::COMPRESSED_DX5:
                return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            case gE::Asset::TextureType::COMPRESSED_DX5_SRGB:
                return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
            case gE::Asset::TextureType::RGBf_32:
                return GL_RGB32F;
            default:
                return GL_RGBA32UI;
        };
    }

    GLenum FormatToPixelType(gE::Asset::TextureType type)
    {
        if (type == TextureType::RGBf_32) return GL_RGB;
        return GL_RGBA; // TODO
    }

    GLenum FormatToSizedFormat(gE::Asset::TextureType type)
    {
        return FormatToPixelFormat(type);
    }

    CompressionRatio FormatToCompressionRatio(gE::Asset::TextureType type)
    {
        if (type == gE::Asset::TextureType::COMPRESSED_DX1 || type == gE::Asset::TextureType::COMPRESSED_DX1_SRGB)
            return {16, 8};
        if (IS_COMPRESSED(type)) return {16, 16};
        if(type == TextureType::RGBf_32) return {1, sizeof(float) * 3};
        return {1, 1};
    }

    uint8_t FormatToPixelSize(TextureType t) {
        if(t == TextureType::RGBf_32) return 12;
        return 1;
    }
}