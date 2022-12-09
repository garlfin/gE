//
// Created by scion on 11/30/2022.
//

#pragma once

#include "../Component.h"
#include "../../Asset/Buffer/Buffer.h"
#include <glm/mat4x4.hpp>

namespace gE::Component
{
    class Camera : public Component
    {
    protected:
        glm::mat4 Projection;
        glm::vec2 ClipPlanes;
    public:
        Camera(gE::Entity* owner, glm::vec2 clipPlanes) : Component(owner), Projection(1.0), ClipPlanes(clipPlanes) {}

        void OnLoad() override { OnUpdate(0.0); UpdateProjection(); }
        void OnRender(double delta) final { OnUpdate(delta); }
        void OnUpdate(double delta) override = 0;
        void OnDestroy() final { }

        [[nodiscard]] const glm::mat4* GetProjection() const { return &Projection; }
        [[nodiscard]] glm::vec2 GetClipPlanes() const { return ClipPlanes; }

        void Use();
    protected:
        virtual void UpdateProjection() = 0;
    };

    struct CameraData
    {
    public:
        CameraData(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& position, const glm::vec4& info)
                : View(view), Projection(projection), Position(position), Info(info)
        {}

        glm::mat4 View;
        glm::mat4 Projection;
        glm::vec3 Position;
    private:
        float pad;
    public:
        glm::vec4 Info;
    };

    class CameraManager final : public ComponentManager<Camera>
    {
    private:
        Buffer<CameraData> p_CameraBuffer;
        Camera* p_CurrentCamera;
        Window* p_Window;
    public:
        explicit CameraManager(Window* window) : ComponentManager<Camera>(), p_CurrentCamera(nullptr),
                                        p_CameraBuffer(window), p_Window(window)
        {
            p_CameraBuffer.Bind(0, BufferTarget::UNIFORM);
        }

        void SetCamera(Camera* camera) { p_CurrentCamera = camera; }
        Camera* GetCamera() { return p_CurrentCamera; }

        void OnUpdate(double delta) override;
    };
}