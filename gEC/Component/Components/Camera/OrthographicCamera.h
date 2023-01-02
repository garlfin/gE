//
// Created by scion on 12/16/2022.
//

#pragma once

#include "Camera.h"

namespace gE::Component
{
    class OrthographicCamera : public Camera
    {
    protected:
        float Size;

    public:
        OrthographicCamera(Entity* owner, float size, glm::vec2 clipPlanes) : Camera(owner, clipPlanes), Size(size) {}

    private:
        void UpdateProjection() override;
    };
}