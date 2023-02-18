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
        ((DemoWindow*) GetWindow())->CubemapManager->Skybox.Render();

        glGenerateTextureMipmap(InternalColor->Get());
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
        if(!Skybox.SkyboxTexture || Skybox.SkyboxTexture->GetSize() != tex->GetSize())
        {
            delete Skybox.SkyboxTexture;
            Skybox.SkyboxTexture = new Asset::TextureCube(tex->GetWindow(), tex->GetSize().x, Asset::TextureType::RGBf_32, tex->GetMipCount());
        }

        Convolute(tex, Skybox.SkyboxTexture);
    }

    void CubemapManager::Convolute(Asset::Texture* src, Asset::Texture* dst)
    {
        _convolutionShader.Use();
        _convolutionBuffer.Bind();
        glProgramUniform1i(_convolutionShader.Get(), glGetUniformLocation(_convolutionShader.Get(), "Skybox"), src->Use(0));
        {
            CameraData data(glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)), glm::perspectiveFov(1.5708f, 1.f, 1.f, 0.01f, 100.f), glm::vec3(0), glm::vec4(0), 0, 0);
            _convolutionBuffer.GetWindow()->CameraManager->GetBuffer()->ReplaceData(&data);
        }
        for(int i = 0; i < src->GetMipCount(); i++)
        {
            auto mipSize = src->GetSize(i);
            glViewport(0, 0, mipSize.x, mipSize.y);
            _convolutionBuffer.Attach(dst, 0, i);

            glProgramUniform2f(_convolutionShader.Get(), glGetUniformLocation(_convolutionShader.Get(), "Data"), (float) i / src->GetMipCount(), src->GetSize().x);

            Skybox.SkyboxVAO->Draw(6);
        }
    }

    void CubemapManager::OnUpdate(double delta)
    {
        ComponentManager::OnUpdate(delta);
    }

    void SkyboxInfo::Render() const
    {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);

        SkyboxShader.Use();
        SkyboxVAO->Draw(SkyboxVAO->GetWindow()->GetStage() == Windowing::Stage::Cubemap ? 6 : 1);
    }
}