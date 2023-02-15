//
// Created by scion on 12/2/2022.
//

#include "Camera.h"
#include "../../../Windowing/Window.h"
#include "../../../Asset/Texture/Texture2D.h"
#include "../../../Asset/Texture/TextureCube.h"

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
        UpdateProjection();
        if(Color && InternalColor)
            glCopyImageSubData(InternalColor->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, Color->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, InternalColor->GetSize().x, InternalColor->GetSize().y, 1);
        Framebuffer->Bind();
        {
            auto* c = GetColor();
            CameraData d(GetView(), GetProjection(), glm::vec3(GetOwner()->GetComponent<Transform>()->Model[3]), glm::vec4(InternalDepth->GetSize(), ClipPlanes), c ? c->GetHandle() : 0, GetDepth()->GetHandle());
            GetWindow()->CameraManager->GetBuffer()->ReplaceData(&d);
        }

        glViewport(0, 0, InternalDepth->GetSize().x, InternalDepth->GetSize().y);
        RenderPass(delta);
    }


    Camera::Camera(gE::Entity* owner, glm::vec2 clipPlanes, glm::uvec2 dimensions, CameraFields fields, Asset::TextureType colorFormat) : Component(owner),
        Projection(1.0), ClipPlanes(clipPlanes) ,
        Framebuffer(owner->GetWindow()->AssetManager.Create<Asset::Framebuffer>()),
        InternalDepth(nullptr), InternalColor(nullptr), Color(nullptr), Depth(nullptr)
    {
        if(fields & CameraFields::CUBEMAP)
            InternalDepth = GetWindow()->AssetManager.Create<Asset::TextureCube>(dimensions.x, Asset::TextureType::DEPTH_32F);
        else
            InternalDepth = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, Asset::TextureType::DEPTH_32F);
        Framebuffer->Attach(InternalDepth, Asset::Framebuffer::DEPTH_STENCIL);

        if(fields & CameraFields::COLOR)
        {
            if(fields & CameraFields::CUBEMAP)
                InternalColor = GetWindow()->AssetManager.Create<Asset::TextureCube>(dimensions.x, colorFormat, 0);
            else
                InternalColor = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, colorFormat);
            Framebuffer->Attach(InternalColor, 0);
        }
        if(fields & CameraFields::COLOR_COPY)
            Color = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, colorFormat);
        if(fields & CameraFields::DEPTH_COPY)
            Depth = GetWindow()->AssetManager.Create<Asset::Texture2D>(dimensions.x, dimensions.y, Asset::TextureType::DEPTH_32F, 1, Asset::TextureFilterMode::NEAREST);

    }

    Camera::~Camera()
    {
        GetWindow()->AssetManager.Destroy(InternalColor);
        GetWindow()->AssetManager.Destroy(InternalDepth);
        GetWindow()->AssetManager.Destroy(Depth);
        GetWindow()->AssetManager.Destroy(Color);
    }

    glm::mat4 CameraManager::GetView() const {
        return glm::inverse(p_CurrentCamera->GetOwner()->GetComponent<Transform>()->Model);
    }
}