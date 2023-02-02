//
// Created by scion on 1/31/2023.
//

#pragma once

#include "Camera.h"
#include "../../../Asset/Buffer/Framebuffer.h"
#include "../../../Asset/Buffer/Renderbuffer.h"
#include "../../../Asset/Texture/TextureCube.h"

namespace gE::Component
{
    class CubemapCamera final : public Camera
    {
    private:
        Asset::Framebuffer* _frameBuffer;
        Asset::TextureCube* _cubeMap;
        Asset::Renderbuffer* _renderBuffer;

    public:
        CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes);
        void OnRender(double delta) override;

    protected:
        void UpdateProjection() override;
    };
}
