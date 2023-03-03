//
// Created by scion on 11/30/2022.
//

#include "PerspectiveCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "../../../Windowing/Window.h"
#include "../../../Windowing/DemoWindow.h"

#define FOV_H_TO_V(fov) float(atan(tan((fov) / 2) / (GetSize().x / GetSize().y)) * 2)

namespace gE::Component
{
    void PerspectiveCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(FOV_H_TO_V(glm::radians(FOV)), (float) GetSize().x, (float) GetSize().y, ClipPlanes.x, ClipPlanes.y);
    }

    void PerspectiveCamera::SetFOV(float fovDegrees, PerspectiveCamera::FOVType type)
    {
        FOV = type == Horizontal ? fovDegrees : glm::degrees(atan(tan(glm::radians(fovDegrees) / 2) / GetWindow()->GetAspectRatio()) * 2);
        UpdateProjection();
    }

    PerspectiveCamera::PerspectiveCamera(Entity* owner, float fov, glm::vec2 planes, FOVType fovType) : Camera(owner, planes, owner->GetWindow()->GetSize(), CameraFields::SCREEN_SPACE_RDY, Asset::TextureType::RGBAf_32), FOV(fov)
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
    }
}
