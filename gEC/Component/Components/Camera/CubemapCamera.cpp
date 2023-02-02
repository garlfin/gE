//
// Created by scion on 1/31/2023.
//

#include "CubemapCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "../../../Windowing/Window.h"

namespace gE::Component
{
    void CubemapCamera::OnRender(double delta)
    {

    }


    void CubemapCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(1.5708f, (float) _cubeMap->GetSize().x, (float) _cubeMap->GetSize().x, ClipPlanes.x, ClipPlanes.y);
    }

    CubemapCamera::CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes) : Camera(owner, clipPlanes)
    {
        _frameBuffer = owner->GetWindow()->AssetManager.Create<Asset::Framebuffer>();
        _renderBuffer = owner->GetWindow()->AssetManager.Create<Asset::Renderbuffer>(size, size, Asset::TextureType::DEPTH_32F);
        _cubeMap = owner->GetWindow()->AssetManager.Create<Asset::TextureCube>(size, Asset::TextureType::RGBAf_32);

        _frameBuffer->Attach(_renderBuffer, Asset::Framebuffer::AttachmentPoint::DEPTH);
        _frameBuffer->Attach(_cubeMap, 0);
    }
}