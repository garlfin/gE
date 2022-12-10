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

namespace gE
{
    struct SkyboxInfo
    {
        Asset::VAO* SkyboxVAO;
        Asset::Shader* SkyboxShader;
        Asset::Texture* SkyboxTexture;

        void Render()
        {
            SkyboxShader->Use();
            SkyboxVAO->Draw(1);
        }
    };

    class DemoWindow : public Window
    {
    private:
        Component::ComponentManager<Component::Transform> TransformManager;
        Asset::AssetManager<Asset::Asset> AssetManager;
        Component::ComponentManager<Component::Behavior> BehaviorManager;
        SkyboxInfo Skybox;
    public:
        DemoWindow(const char* title, uint32_t width, uint32_t height, Result* result);

        void Load() override;
        void Update(double delta) override;
        void Render(double delta) override;
    };
}
