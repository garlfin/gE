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
        OrthographicCamera(Entity* owner, float size, glm::vec2 clipPlanes, glm::uvec2 resolution, CameraFields fields, Asset::TextureType format) : Camera(owner, clipPlanes, resolution, fields, format), Size(size) {}
        float GetSize() const { return Size; }

    private:
        void UpdateProjection() override;
    };
}
