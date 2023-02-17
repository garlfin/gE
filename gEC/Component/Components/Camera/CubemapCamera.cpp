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
        {
            auto* c = GetColor();
            CameraData d(GetView(), GetProjection(), GetOwner()->GetComponent<Transform>()->Location, glm::vec4(InternalDepth->GetSize(), ClipPlanes), c ? c->GetHandle() : 0, GetDepth()->GetHandle());
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

    CubemapManager::CubemapManager(Window* window) : ComponentManager<CubemapCamera>(window), CubemapBuffer(window->AssetManager.Create<Buffer<CubemapData>>(CMMANAGER_MAX_CUBEMAPS))
    {
        CubemapBuffer->Bind(3, BufferTarget::UNIFORM);
    }
}