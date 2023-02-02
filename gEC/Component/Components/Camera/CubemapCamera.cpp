//
// Created by scion on 1/31/2023.
//

#include "CubemapCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "../../../Windowing/Window.h"

namespace gE::Component
{
    void CubemapCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(1.5708f, (float) GetSize().x, (float) GetSize().x, ClipPlanes.x, ClipPlanes.y);
    }

    CubemapCamera::CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes) : Camera(owner, clipPlanes, glm::uvec2(size), CameraFields::COLOR, Asset::TextureType::RGBf_32)
    {
    }
}