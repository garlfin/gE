//
// Created by scion on 11/30/2022.
//

#pragma once
#include "Camera.h"

namespace gE::Component
{
    class PerspectiveCamera final : public Camera
    {
    private:
        float FOV;

    public:
        enum FOVType : uint8_t
        {
            Vertical,
            Horizontal
        };

        PerspectiveCamera(Entity* owner, float fov, glm::vec2 clipPlane, FOVType type = Horizontal);
        void SetFOV(float fovDegrees, FOVType type = Horizontal);

    protected:
        void RenderPass(double delta) override;
        void UpdateProjection() override;
    };
}
