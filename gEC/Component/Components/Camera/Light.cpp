//
// Created by scion on 12/16/2022.
//

#include "Light.h"
#include "../../../Windowing/Window.h"

const float BORDER_COLOR[] {1, 1, 1, 1};

namespace gE::Component
{
    Light::~Light()
    {
        window->AssetManager.Remove(ShadowMap);
    }

    Light::Light(Entity* entity, Asset::Texture* shadowMap) : window(entity->GetWindow()), ShadowMap(shadowMap)
    , Framebuffer(window)
    {
        glTextureParameterfv(shadowMap->Get(), GL_TEXTURE_BORDER_COLOR, BORDER_COLOR);
        glTextureParameteri(shadowMap->Get(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(shadowMap->Get(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTextureParameteri(shadowMap->Get(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTextureParameteri(shadowMap->Get(), GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        glNamedFramebufferReadBuffer(Framebuffer.Get(), GL_NONE);
        glNamedFramebufferDrawBuffer(Framebuffer.Get(), GL_NONE);
        Framebuffer.Attach(shadowMap, Asset::Framebuffer::DEPTH);
    }
}