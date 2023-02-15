//
// Created by scion on 1/31/2023.
//

#include "CubemapCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "../../../Windowing/Window.h"
#include "../../../Windowing/DemoWindow.h"

namespace gE::Component
{
    void CubemapCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(1.5708f, (float) GetSize().x, (float) GetSize().x, ClipPlanes.x, ClipPlanes.y);
    }

    CubemapCamera::CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes) : Camera(owner, clipPlanes, glm::uvec2(size), CameraFields::CUBEMAP_RDY, Asset::TextureType::RGBf_32)
    {
    }

    glm::mat4 CubemapCamera::GetView() const
    {
        auto* transform = GetOwner()->GetComponent<gE::Component::Transform>();
        return glm::lookAt(transform->Location, transform->Location + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
    }

    void CubemapCamera::RenderPass(double delta)
    {
        DemoWindow* window = (DemoWindow*) GetWindow();
        auto* sunTransform = window->Sun->GetOwner()->GetComponent<gE::Component::Transform>();
        glm::vec3 sunDir;
        sunDir.x =  cos(glm::radians(sunTransform->Rotation.x)) * sin(glm::radians(sunTransform->Rotation.y));
        sunDir.y = -sin(glm::radians(sunTransform->Rotation.x));
        sunDir.z =  cos(glm::radians(sunTransform->Rotation.x)) * cos(glm::radians(sunTransform->Rotation.y));
        sunTransform->Location = glm::normalize(sunDir) * 25.0f;
        sunTransform->Location += glm::floor(GetOwner()->GetComponent<gE::Component::Transform>()->Location / 1.0f) * 1.0f;

        {
            DemoUBO ubo(window->Skybox.SkyboxTexture, window->Sun, window->BRDF, window->GetFrame());
            window->DemoUniformBuffer->ReplaceData(&ubo);
        }

        window->Sun->OnRender(0);

        Framebuffer->Bind();
        {
            auto* c = GetColor();
            CameraData d(GetView(), GetProjection(), glm::vec3(GetOwner()->GetComponent<Transform>()->Model[3]), glm::vec4(InternalDepth->GetSize(), ClipPlanes), c ? c->GetHandle() : 0, GetDepth()->GetHandle());
            GetWindow()->CameraManager->GetBuffer()->ReplaceData(&d);
        }

        glViewport(0, 0, InternalDepth->GetSize().x, InternalDepth->GetSize().y);

        GetWindow()->SetStage(Windowing::Stage::Cubemap);
        glDepthMask(true);
        glColorMask(true, true, true, true);
        glClear(GL_DEPTH_BUFFER_BIT);

        GetWindow()->MeshManager->OnRender();
        ((DemoWindow*) GetWindow())->Skybox.Render();

        glGenerateTextureMipmap(InternalColor->Get());
    }

    CubemapManager::CubemapManager(Window* window) : ComponentManager<CubemapCamera>(window), CubemapBuffer(window->AssetManager.Create<Buffer<CubemapData>>())
    {
    }
}