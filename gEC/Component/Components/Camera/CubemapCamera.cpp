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

    CubemapCamera::CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes) : Camera(owner, clipPlanes, glm::uvec2(size), CameraFields::CUBEMAP_RDY, Asset::TextureType::RGBAf_32)
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
            CameraData d(GetView(), glm::mat4(1), GetProjection(), glm::mat4(1), GetOwner()->GetComponent<Transform>()->Location, glm::vec4(InternalDepth->GetSize(), ClipPlanes), c ? c->GetHandle() : 0, GetDepth()->GetHandle());
            GetWindow()->CameraManager->GetBuffer()->ReplaceData(&d);
        }

        glViewport(0, 0, InternalDepth->GetSize().x, InternalDepth->GetSize().y);

        GetWindow()->SetStage(Windowing::Stage::CubemapPreZ);
        glDepthMask(true);
        glColorMask(false, false, false, false);
        glClear(GL_DEPTH_BUFFER_BIT);

        GetWindow()->MeshManager->OnRender();

        GetWindow()->SetStage(Windowing::Stage::Cubemap);
        glDepthMask(false);
        glColorMask(true, true, true, true);
        glDepthFunc(GL_EQUAL);

        GetWindow()->MeshManager->OnRender();
        ((DemoWindow*) GetWindow())->CubemapManager->Skybox.Render();
    }

    CubemapManager::CubemapManager(Window* window) : ComponentManager<CubemapCamera>(window),
                                                     CubemapBuffer(window),
                                                     Skybox(window, nullptr),
                                                     _convolutionBuffer(window),
                                                     _convolutionShader(window, "../res/shader/skybox.vert", "../res/shader/convolution.frag", Asset::CullMode::NEVER, Asset::DepthFunction::ALWAYS)
    {
        CubemapBuffer.Bind(3, BufferTarget::UNIFORM);
    }

    void CubemapManager::UpdateSkybox(Asset::Texture* tex)
    {
        Skybox.SkyboxTexture = tex;
        return;

        /*if(!Skybox.SkyboxTexture || Skybox.SkyboxTexture->GetSize() != tex->GetSize())
        {
            delete Skybox.SkyboxTexture;
            Skybox.SkyboxTexture = new Asset::TextureCube(tex->GetWindow(), tex->GetSize().x, Asset::TextureType::RGBf_32, tex->GetMipCount());
        }

        Convolute(tex, Skybox.SkyboxTexture);*/
    }

    void CubemapManager::Convolute(Asset::Texture* src, Asset::Texture* dst)
    {
        _convolutionShader.Use();
        _convolutionBuffer.Bind();
        glProgramUniform1i(_convolutionShader.Get(), glGetUniformLocation(_convolutionShader.Get(), "Skybox"), src->Use(0));
        {
            CameraData data(glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)), glm::mat4(1), glm::perspectiveFov(1.5708f, 1.f, 1.f, 0.01f, 100.f), glm::mat4(1), glm::vec3(0), glm::vec4(0), 0, 0);
            _convolutionBuffer.GetWindow()->CameraManager->GetBuffer()->ReplaceData(&data);
        }
        for(int i = 0; i < dst->GetMipCount(); i++)
        {
            auto mipSize = dst->GetSize(i);
            glViewport(0, 0, mipSize.x, mipSize.y);
            _convolutionBuffer.Attach(dst, 0, i);

            glProgramUniform2f(_convolutionShader.Get(), glGetUniformLocation(_convolutionShader.Get(), "Data"), (float) i / (dst->GetMipCount() - 1), dst->GetSize().x);

            Skybox.SkyboxVAO->Draw(6);
        }
    }

    void SkyboxInfo::Render() const
    {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);

        SkyboxShader.Use();
        SkyboxVAO->Draw(SkyboxVAO->GetWindow()->GetStage() == Windowing::Stage::Cubemap ? 6 : 1);
    }
}