//
// Created by scion on 12/16/2022.
//

#pragma once

#include "OrthographicCamera.h"

namespace gE::Component
{
    class DirectionalLight : public OrthographicCamera
    {
    public:
        DirectionalLight(Entity* entity, uint32_t size, float cameraSize);

        void RenderPass(double delta) override;
    };
}
