//
// Created by scion on 12/16/2022.
//

#pragma once

#include "OrthographicCamera.h"
#include "Light.h"

namespace gE::Component
{
    class DirectionalLight : public OrthographicCamera, public Light
    {
    public:
        DirectionalLight(Entity* entity, uint32_t size);

        void OnRender(double delta) override;
        void Use() final;
    };
}
