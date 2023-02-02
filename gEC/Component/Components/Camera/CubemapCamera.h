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
    public:
        CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes);

    protected:
        void UpdateProjection() override;
    };
}
