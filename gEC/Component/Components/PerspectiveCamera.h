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
        PerspectiveCamera(Entity* owner, float fov, glm::vec2 clipPlane);

        enum FOVType : uint8_t
        {
            Vertical,
            Horizontal
        };

        void SetFOV(float fovDegrees, FOVType type = Horizontal);
        void OnUpdate(double delta) override {};

    protected:
        void UpdateProjection() override;
    };
}
