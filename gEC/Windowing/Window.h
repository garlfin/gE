//
// Created by scion on 11/2/2022.
//

#pragma once

#include <glm/vec2.hpp>
#include "../Result.h"
#include "../Component/Entity.h"
#include "../Asset/IncludeManager.h"
#include "../Component/Components/Camera/Camera.h"
#include "../Asset/VAO/MeshManager.h"
#include "Stage.h"
#include "../Asset/Shader/Shader.h"
#include "../Component/Components/Transform.h"
#include "../Component/Components/Behavior.h"

class GLFWwindow;

namespace gE
{
    Result Init(int32_t, int32_t, bool = false);
    Result Destroy();

    class Window
    {
    private:
        Asset::ShaderStage* p_MissingFrag;
        Asset::Shader* p_MissingShader;
    public:
        Window(const char* const title, const uint32_t width, const uint32_t height, Result* const result);
        ~Window();

        void Run();
        [[nodiscard]] inline GLFWwindow* GetWindow() const { return GLFWWindow; }

        Asset::AssetManager<Entity> EntityManager;
        Asset::IncludeManager IncludeManager;
        Component::CameraManager* CameraManager;
        Asset::MeshManager* MeshManager;
        Component::ComponentManager<Component::Component> ComponentManager;
        Component::TransformManager* TransformManager;
        Asset::AssetManager<Asset::Asset> AssetManager;
        Component::ComponentManager<Component::Behavior> BehaviorManager;

        Asset::Framebuffer* BlitBuffer;
        Asset::Shader* PassthroughShader;
        Asset::VAO* PassthroughVAO;

        [[nodiscard]] float GetAspectRatio() const { return (float) Size.x / Size.y; }
        [[nodiscard]] glm::u32vec2 GetSize() const { return Size; }
        [[nodiscard]] Windowing::Stage GetStage() const {return Stage; }
        [[nodiscard]] const Asset::Shader* GetDefaultShader() const { return p_MissingShader; }
        [[nodiscard]] Asset::ShaderStage* GetDefaultFragment() const { return p_MissingFrag; }

        virtual void SetStage(Windowing::Stage stage, bool replaceBuffer = true);
        uint32_t GetFrame() const { return Frame; }

    protected:
        virtual void Load() = 0;
        virtual void Render(double delta) = 0;
        virtual void Update(double delta) = 0;

        const char* Title;
        glm::u32vec2 Size;
        GLFWwindow* GLFWWindow;
        Windowing::Stage Stage;
        int32_t Frame;
    };
}
