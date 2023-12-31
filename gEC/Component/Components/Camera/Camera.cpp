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
        auto* pCam = GetWindow()->CameraManager->GetCamera();
        Use();
        GetWindow()->MeshManager->OnUpdate(0);
        UpdateProjection();
        Framebuffer->Bind();
        {
            CameraData d(GetView(), PreviousView, GetProjection(), PreviousProjection, glm::vec3(GetOwner()->GetComponent<Transform>()->Model[3]), glm::vec4(InternalDepth->GetSize(), ClipPlanes), GetColor() ? GetColor()->GetHandle() : 0, Depth ? Depth->GetHandle() : 0);
            GetWindow()->CameraManager->GetBuffer()->ReplaceData(&d);
        }

        glViewport(0, 0, InternalDepth->GetSize().x, InternalDepth->GetSize().y);
        RenderPass(delta);

        PreviousView = GetView();
        PreviousProjection = GetProjection();
        pCam->Use();
    }


    Camera::Camera(gE::Entity* owner, glm::vec2 clipPlanes, glm::uvec2 dimensions, CameraFields fields, Asset::TextureType colorFormat) : Component(owner),
        Projection(1.0), ClipPlanes(clipPlanes) ,
        Framebuffer(new Asset::Framebuffer(GetWindow())),
        InternalDepth(nullptr), InternalColor(nullptr), Color(nullptr), Depth(nullptr), Velocity(0), PreviousView(1)
    {
        if(fields & CameraFields::CUBEMAP)
            InternalDepth = new Asset::TextureCube(GetWindow(), dimensions.x, Asset::TextureType::DEPTH_32f);
        else
            InternalDepth = new Asset::Texture2D(GetWindow(),dimensions.x, dimensions.y, Asset::TextureType::DEPTH_32f);
        Framebuffer->Attach(InternalDepth, Asset::Framebuffer::DEPTH);

        if(fields & CameraFields::COLOR)
        {
            if(fields & CameraFields::CUBEMAP)
                InternalColor = new Asset::TextureCube(GetWindow(), dimensions.x, colorFormat, (uint8_t) 0);
            else
                InternalColor = new Asset::Texture2D(GetWindow(), dimensions.x, dimensions.y, colorFormat, 1, Asset::TextureFilterMode::LINEAR, Asset::TextureWrapMode::EDGE);
            Framebuffer->Attach(InternalColor, 0);
        }

        if(fields & CameraFields::COLOR_COPY)
            Color = new Asset::Texture2D(GetWindow(), dimensions.x, dimensions.y, colorFormat, 1, Asset::TextureFilterMode::LINEAR, Asset::TextureWrapMode::EDGE);
        if(fields & CameraFields::DEPTH_COPY)
            Depth = new Asset::Texture2D(GetWindow(), dimensions.x, dimensions.y, Asset::TextureType::DEPTH_32f, 1, Asset::TextureFilterMode::NEAREST);

        if(fields & CameraFields::VELOCITY)
        {
            Velocity = new Asset::Texture2D(GetWindow(), dimensions.x, dimensions.y, Asset::TextureType::RGf_32);
            Framebuffer->Attach(Velocity, 1);

            static GLenum WRITE_BUFS[2] { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
            glNamedFramebufferDrawBuffers(Framebuffer->Get(), 2, WRITE_BUFS);
        }
    }

    Camera::~Camera()
    {
        delete Color;
        delete InternalColor;
        delete Depth;
        delete Velocity;
        delete InternalDepth;
    }

    glm::mat4 CameraManager::GetView() const {
        return glm::inverse(p_CurrentCamera->GetOwner()->GetComponent<Transform>()->Model);
    }
}