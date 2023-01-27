//
// Created by scion on 12/16/2022.
//

#include "../../../Windowing/DemoWindow.h"
#include <iostream>
#include "DirectionalLight.h"
#include "../../../Asset/Texture/Texture2D.h"

namespace gE::Component
{
    DirectionalLight::DirectionalLight(Entity* entity, uint32_t size, float cameraSize) : OrthographicCamera(entity, cameraSize, glm::vec2(0.01, 30)), Light(entity, GetWindow()->AssetManager.Create<Asset::Texture2D>(size, size, Asset::TextureType::DEPTH_32F, 1, Asset::TextureFilterMode::LINEAR))
    {

    }

    void DirectionalLight::OnRender(double delta)
    {
        Use();
        // Setup

        Framebuffer.Bind();

        glDepthMask(true);
        glColorMask(false, false, false, false);
        glViewport(0, 0, ShadowMap->GetSize().x, ShadowMap->GetSize().y);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Render
        GetWindow()->CameraManager->OnUpdate(0);
        GetWindow()->MeshManager->OnUpdate(0);
        GetWindow()->MeshManager->OnRender();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void DirectionalLight::Use()
    {
        Camera::Use();
        //((DemoWindow*)GetWindow()).
    }

}