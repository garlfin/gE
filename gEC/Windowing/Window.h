//
// Created by scion on 11/2/2022.
//

#pragma once

#include <glm/vec2.hpp>
#include "../Result.h"
#include "../Component/Entity.h"
#include "../Asset/IncludeManager.h"
#include "../Component/Components/Camera.h"
#include "../Asset/VAO/MeshManager.h"

class GLFWwindow;

namespace gE
{
    Result Init(int32_t, int32_t, bool = false);
    Result Destroy();

    class Window
    {
    public:
        Window(const char* const title, const uint32_t width, const uint32_t height, Result* const result);
        ~Window();

        void Run();
        [[nodiscard]] inline GLFWwindow* GetWindow() const { return p_Window; }
        Asset::AssetManager<Entity> EntityManager;
        Asset::IncludeManager IncludeManager;
        Component::CameraManager* CameraManager;
        Asset::MeshManager* MeshManager;
        Component::ComponentManager<Component::Component> ComponentManager;

        [[nodiscard]] float GetAspectRatio() const { return (float) p_Size.x / p_Size.y; }
        [[nodiscard]] glm::u32vec2 GetSize() const { return p_Size; }

    protected:
        virtual void Load() = 0;
        virtual void Render(double delta) = 0;
        virtual void Update(double delta) = 0;

        const char* p_Title;
        glm::u32vec2 p_Size;
        GLFWwindow* p_Window;
    };
}
