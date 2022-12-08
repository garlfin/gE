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
    public:
        Renderbuffer(Window* window, uint32_t width, uint32_t height, TextureType type);
        ~Renderbuffer() override;

    private:
        void BindToFramebuffer(Framebuffer *framebuffer, GLenum location, uint32_t mip) override;
    };
}