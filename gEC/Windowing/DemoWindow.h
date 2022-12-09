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

namespace gE
{
    class DemoWindow : public Window
    {
    private:
        Component::ComponentManager<Component::Transform> TransformManager;
        Asset::AssetManager<Asset::Asset> AssetManager;
        Component::ComponentManager<Component::Behavior> BehaviorManager;
    public:
        DemoWindow(const char* const title, const uint32_t width, const uint32_t height, Result* const result);

        void Load() override;
        void Update(double delta) override;
        void Render(double delta) override;
    };
}
