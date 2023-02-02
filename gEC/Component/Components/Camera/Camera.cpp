//
// Created by scion on 12/2/2022.
//

#include "Camera.h"
#include "../../../Windowing/Window.h"
#include "../../../Asset/Texture/Texture2D.h"

namespace gE::Component
{
    void Camera::Use()
    {
        GetWindow()->CameraManager->SetCamera(this);
    }

    glm::mat4 Camera::GetView() const
    {
        return glm::inverse(GetOwner()->GetComponent<Transform>()->Model);
    }

    void Camera::OnRender(double delta)
    {
        if(Color)
            glCopyImageSubData(InternalColor->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, Color->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, InternalColor->GetSize().x, InternalColor->GetSize().y, 1);
        Framebuffer->Bind();
        {
            auto* c = GetColor();
            CameraData d(GetView(), GetProjection(), GetOwner()->GetComponent<Transform>()->Location, glm::vec4(InternalDepth->GetSize(), ClipPlanes), c ? c->GetHandle() : 0, GetDepth()->GetHandle());
            GetWindow()->CameraManager->GetBuffer()->ReplaceData(&d);
        }

        glViewport(0, 0, InternalDepth->GetSize().x, InternalDepth->GetSize().y);
        RenderPass(delta);
    }


    Camera::Camera(gE::Entity* owner, glm::vec2 clipPlanes, glm::uvec2 dimensions, CameraFields fields, Asset::TextureType colorFormat) : Component(owner),
        Projection(1.0), ClipPlanes(clipPlanes) ,
        Framebuffer(owner->GetWindow()->AssetManager.Create<Asset::Framebuffer>()),
        InternalDepth(owner->GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, Asset::TextureType::DEPTH_32F)),
        InternalColor(nullptr), Color(nullptr), Depth(nullptr)
    {
        if (fields == CameraFields::COLOR)
        {
            InternalColor = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, colorFormat);
            Framebuffer->Attach(InternalColor, 0);
        }
        else if (fields == CameraFields::SCREEN_SPACE_READY)
        {
            InternalColor = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, colorFormat);
            Color = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, colorFormat);
            Depth = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y,
                                                                       Asset::TextureType::DEPTH_32F, 1, Asset::TextureFilterMode::NEAREST, Asset::TextureWrapMode::BORDER);
            Framebuffer->Attach(InternalColor, 0);
        }

        Framebuffer->Attach(InternalDepth, Asset::Framebuffer::DEPTH);
    }

    Camera::~Camera()
    {
        GetWindow()->AssetManager.Remove(InternalColor);
        GetWindow()->AssetManager.Remove(InternalDepth);
        GetWindow()->AssetManager.Remove(Depth);
        GetWindow()->AssetManager.Remove(Color);
    }

    glm::mat4 CameraManager::GetView() const {
        return glm::inverse(p_CurrentCamera->GetOwner()->GetComponent<Transform>()->Model);
    }
}