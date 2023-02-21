//
// Created by scion on 11/30/2022.
//

#include "PerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "../../../Windowing/Window.h"
#include "../../../Windowing/DemoWindow.h"
#include "../../../Asset/Texture/Texture2D.h"

#define FOV_H_TO_V(fov) atan(tan((fov) / 2) / GetWindow()->GetAspectRatio()) * 2

namespace gE::Component
{
    void PerspectiveCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(glm::radians(FOV), (float) GetWindow()->GetSize().x, (float) GetWindow()->GetSize().y, ClipPlanes.x, ClipPlanes.y);
    }

    void PerspectiveCamera::SetFOV(float fovDegrees, PerspectiveCamera::FOVType type)
    {
        FOV = type == Horizontal ? fovDegrees : glm::degrees(atan(tan(glm::radians(fovDegrees) / 2) / GetWindow()->GetAspectRatio()) * 2);
        UpdateProjection();
    }

    PerspectiveCamera::PerspectiveCamera(Entity* owner, float fov, glm::vec2 planes, FOVType fovType) : Camera(owner, planes, owner->GetWindow()->GetSize(), CameraFields::SCREEN_SPACE_RDY, Asset::TextureType::RGBAf_32), FOV(fov),
    _TAABufferTex(new Asset::Texture2D(owner->GetWindow(), owner->GetWindow()->GetSize().x, owner->GetWindow()->GetSize().y, Asset::TextureType::RGBAf_32))
    {
        SetFOV(fov, fovType);
    }

    void PerspectiveCamera::RenderPass(double delta)
    {
        GetWindow()->SetStage(Windowing::Stage::PreZ);

        glColorMask(false, false, false, false);
        glDepthMask(true);
        glClear(GL_DEPTH_BUFFER_BIT);

        GetWindow()->MeshManager->OnUpdate(0);
        GetWindow()->MeshManager->OnRender();

        glCopyImageSubData(InternalDepth->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, Depth->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, InternalColor->GetSize().x, InternalColor->GetSize().y, 1);

        GetWindow()->SetStage(Windowing::Stage::Render);
        Framebuffer->Bind();

        glDepthMask(false);
        glDepthFunc(GL_EQUAL);
        glColorMask(true, true, true, true);

        GetWindow()->MeshManager->OnRender();
        ((DemoWindow*) GetWindow())->CubemapManager->Skybox.Render();

        GetWindow()->SetStage(Windowing::Stage::PostProcess);
        glViewport(0, 0, GetSize().x, GetSize().y);
        Framebuffer->Attach(_TAABufferTex, 0);

        auto* taaShader = ((DemoWindow*) GetWindow())->TAAShader;
        taaShader->Use();

        glProgramUniform1i(taaShader->Get(), 0, InternalColor->Use(0));
        glProgramUniform1i(taaShader->Get(), 1, Color->Use(1));
        glProgramUniform1i(taaShader->Get(), 2, Velocity->Use(2));

        GetWindow()->PassthroughVAO->Draw(1);

        Framebuffer->Attach(InternalColor, 0);

        glCopyImageSubData(_TAABufferTex->Get(), GL_TEXTURE_2D, 0, 0, 0, 0, InternalColor->Get(), GL_TEXTURE_2D, 0, 0, 0,0, GetSize().x, GetSize().y, 1);
    }
}
