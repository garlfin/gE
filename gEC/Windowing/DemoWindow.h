//
// Created by scion on 11/2/2022.
//

#pragma once

#include "Window.h"
#include "gEModel.h"
#include "../Component/Components/Transform.h"
#include "../Asset/Texture/Texture.h"
#include "../Component/Components/Renderer.h"
#include "../../res/script/CameraMovement.h"
#include "../Asset/Shader/Shader.h"
#include "../Component/Components/Camera/DirectionalLight.h"

namespace gE
{
    struct SkyboxInfo
    {
        Asset::VAO* SkyboxVAO;
        Asset::Shader* SkyboxShader;
        Asset::Texture* SkyboxTexture;

        void Render()
        {
            glDepthFunc(GL_LEQUAL);
            glDisable(GL_CULL_FACE);

            SkyboxShader->Use();
            SkyboxVAO->Draw(1);
        }
    };

    struct DemoUBO
    {
    public:
        DemoUBO(Asset::Texture* sky, Component::DirectionalLight* sun, Asset::Texture* brdf, int32_t frame);

        uint64_t SkyboxID;
        uint64_t ShadowID;
        glm::vec4 SunInfo; // Direction, ShadowSize
        glm::mat4 SunMatrix;
        uint64_t BRDFID;
        uint64_t pad;
        int32_t Frame;
    };

    class DemoWindow : public Window
    {
    public:
        DemoWindow(const char* title, uint32_t width, uint32_t height, Result* result);

        void Load() override;
        void Update(double delta) override;
        void Render(double delta) override;

        SkyboxInfo Skybox;
        Component::ComponentManager<Component::Camera> LightManager;
        Buffer<DemoUBO>* DemoUniformBuffer;
        Component::DirectionalLight* Sun;
        Asset::Texture* BRDF;
    };
}
