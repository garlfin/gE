//
// Created by scion on 12/7/2022.
//

#pragma once

#include "../GLAsset.h"

namespace gE::Asset
{
    class Framebuffer;
    class Attachable
    {
        friend class Framebuffer;
    private:
        virtual void BindToFramebuffer(Framebuffer* framebuffer, GLenum location, uint32_t mip = 0) = 0;
    };

    class Framebuffer final : public GLAsset
    {
    public:
        enum FramebufferTarget
        {
            DRAW = 0x8CA8,
            READ = 0x8CA9,
            ALL = 0x8D40
        };

        enum AttachmentPoint
        {
            DEPTH = 0x8D00,
            STENCIL = 0x8D20,
            DEPTH_STENCIL = 0x821A,
        };

        explicit Framebuffer(Window* window);
        ~Framebuffer() override;

        void Attach(Attachable* attachment, AttachmentPoint point, uint32_t mip = 0);
        void Attach(Attachable* attachable, uint32_t colorSlot, uint32_t mip = 0);
        void Bind(FramebufferTarget = ALL);
        Attachable* GetAttachment(uint32_t i) { return p_attachments[i]; }

    private:
        Attachable* p_attachments[10];
    };
}
