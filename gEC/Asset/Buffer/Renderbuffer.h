//
// Created by scion on 12/7/2022.
//

#pragma once

#include "../GLAsset.h"
#include "../Texture/Texture.h"

namespace gE::Asset
{
    class Renderbuffer : public GLAsset, public Attachable
    {
    private:
        uint32_t p_Width, p_Height;
        void BindToFramebuffer(Framebuffer *framebuffer, GLenum location, uint32_t mip) override;
    public:
        Renderbuffer(Window* window, uint32_t width, uint32_t height, TextureType type);
        ~Renderbuffer() override;

        [[nodiscard]] glm::u32vec2 GetSize() const { return {p_Width, p_Height}; }
    };
}