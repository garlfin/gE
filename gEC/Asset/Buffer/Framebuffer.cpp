//
// Created by scion on 12/7/2022.
//

#include "GLAD/glad.h"
#include "Framebuffer.h"

static unsigned BIND_COUNT = 0;

namespace gE::Asset
{
    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &ID);
    }

    Framebuffer::Framebuffer(Window *window) : GLAsset(window), p_attachments()
    {
        glCreateFramebuffers(1, &ID);
    }

    void Framebuffer::Bind(Framebuffer::FramebufferTarget target)
    {
        glBindFramebuffer((GLenum) target, ID);
    }

    void Framebuffer::Attach(Attachable *attachment, Framebuffer::AttachmentPoint point, uint32_t mip)
    {
        switch(point)
        {
            case AttachmentPoint::DEPTH:
                p_attachments[0] = attachment;
                break;
            case AttachmentPoint::STENCIL:
                p_attachments[1] = attachment;
                break;
            case AttachmentPoint::DEPTH_STENCIL:
                p_attachments[1] = p_attachments[0] = attachment;
                break;
            default:
                p_attachments[2 + (GL_COLOR_ATTACHMENT0 - point)] = attachment;
        }
        attachment->BindToFramebuffer(this, point, mip);
    }

    void Framebuffer::Attach(Attachable *attachment, uint32_t colorSlot, uint32_t mip)
    {
        Attach(attachment, (Framebuffer::AttachmentPoint) (GL_COLOR_ATTACHMENT0 + colorSlot), mip);
    }
}