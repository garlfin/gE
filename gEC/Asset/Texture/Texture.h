//
// Created by scion on 11/3/2022.
//

#pragma once

#include "../GLAsset.h"
#include "glm/vec2.hpp"
#include "GLAD/glad.h"
#include "../Buffer/Framebuffer.h"

#define DIV_CEIL(x, y) (((x) + (y) - 1) / (y))
#define IS_COMPRESSED(t) (t <= TextureType::COMPRESSED_DX5 || (t >= TextureType::COMPRESSED_DX1_SRGB && t <= TextureType::COMPRESSED_DX5_SRGB))


namespace gE::Asset
{
    enum class TextureFilterMode
    {
        LINEAR = 0,
        NEAREST = 1,
    };

    enum class TextureType : uint64_t
    {
        COMPRESSED_DX1 = 7,
        COMPRESSED_DX2 = 8,
        COMPRESSED_DX3 = 9,
        COMPRESSED_DX5 = 11,
        COMPRESSED_DX1_SRGB = 51,
        COMPRESSED_DX2_SRGB = 52,
        COMPRESSED_DX3_SRGB = 53,
        COMPRESSED_DX5_SRGB = 55,
        RGBAf_32,
        RGBf_32 = 9042521072232306,
        RGBAi_32,
        RGBi_32,
    };

    enum class AccessMode : uint64_t
    {
        READ = 0x88B8,
        WRITE = 0x88B9,
        ALL = 0x88BA,
    };

    class Texture : public GLAsset, public Attachable
    {
    protected:
        uint64_t Handle = 0;
        uint32_t Width;
        uint32_t Height;
        uint8_t MipMapCount = 0;
        TextureType Format;
    public:
        Texture(Window* window, uint32_t width, uint32_t height, TextureType format, uint8_t mipCount = 0)
        : GLAsset(window), Width(width), Height(height), Format(format), MipMapCount(mipCount)
        {
            if(!mipCount) CalculateMipCount();
        }
        ~Texture() override;

        [[nodiscard]] glm::uvec2 GetSize(uint8_t mip = 0) const { return {Width >> mip, Height >> mip}; }
        [[nodiscard]] uint64_t GetHandle() { if(!Handle) CreateHandle(); return Handle; }

        void CalculateMipCount();
        void CreateHandle();
        virtual uint32_t Use(uint32_t slot);
        virtual uint32_t Bind(uint32_t slot, AccessMode accessMode, uint8_t level = 0);
        virtual void Use() = 0;

    private:
        void BindToFramebuffer(Framebuffer *framebuffer, GLenum location, uint32_t mip) final;
    };

    struct CompressionRatio
    {
        uint8_t Divisor;
        uint8_t Multiplier;

        CompressionRatio(uint8_t d, uint8_t m) : Divisor(d), Multiplier(m) {}

        [[nodiscard]] inline uint32_t CalculatePixelCount(glm::uvec2 size) { return DIV_CEIL(size.x * size.y, Divisor) * Multiplier; }
    };

    [[nodiscard]] GLenum FormatToPixelFormat(TextureType);

    [[nodiscard]] GLenum FormatToPixelType(TextureType);

    [[nodiscard]] GLenum FormatToSizedFormat(TextureType);

    [[nodiscard]] CompressionRatio FormatToCompressionRatio(TextureType);

    [[nodiscard]] uint8_t FormatToPixelSize(TextureType);
}