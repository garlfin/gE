//
// Created by scion on 12/16/2022.
//

#pragma once

#include "Camera.h"
#include "../../../Asset/Buffer/Framebuffer.h"
#include "../../../Asset/Texture/Texture.h"

namespace gE::Component
{
    class Light
    {
    private:
        Window* window;

    protected:
        Asset::Texture* ShadowMap;
        Asset::Framebuffer Framebuffer;

    public:
        Light(Entity* entity, Asset::Texture* shadowMap);

        [[nodiscard]] Asset::Texture* GetShadowMap() const { return ShadowMap; }

        ~Light();

    };
}
