//
// Created by scion on 12/7/2022.
//

#include "Renderbuffer.h"

gE::Asset::Renderbuffer::~Renderbuffer()
{
    glDeleteRenderbuffers(1, &ID);
}

gE::Asset::Renderbuffer::Renderbuffer(gE::Window *window, uint32_t width, uint32_t height, gE::Asset::TextureType type)
                                    : GLAsset(window)
{
    glCreateRenderbuffers(1, &ID);
    glNamedRenderbufferStorage(ID, FormatToSizedFormat(type), width, height);
}

void gE::Asset::Renderbuffer::BindToFramebuffer(gE::Asset::Framebuffer *framebuffer, GLenum location, uint32_t mip)
{
    glNamedFramebufferRenderbuffer(framebuffer->Get(), location, GL_RENDERBUFFER, ID);
}

