//
// Created by scion on 12/16/2022.
//

#include "../../../Windowing/DemoWindow.h"
#include <iostream>
#include "DirectionalLight.h"
#include "../../../Asset/Texture/Texture2D.h"

namespace gE::Component
{
    DirectionalLight::DirectionalLight(Entity* entity, uint32_t size, float cameraSize) : OrthographicCamera(entity, cameraSize, glm::vec2(0.01, 50), glm::uvec2(size), CameraFields::DEPTH, Asset::TextureType::RGBAf_32)
    {

    }

    void DirectionalLight::RenderPass(double delta)
    {
        GetWindow()->SetStage(Windowing::Stage::Shadow);

        glClearColor(0, 0, 1, 1);

        glDepthMask(true);
        glColorMask(false, false, false, false);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Render
        GetWindow()->MeshManager->OnUpdate(0);
        GetWindow()->MeshManager->OnRender();
    }
}