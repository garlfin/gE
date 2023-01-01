//
// Created by scion on 11/30/2022.
//

#pragma once

#include "../../Component.h"
#include "../../../Asset/Buffer/Buffer.h"
#include "../../../Utility/Frustum.h"
#include "glm/mat4x4.hpp"

namespace gE::Component
{
    class Camera : public Component
    {
    protected:
        glm::mat4 Projection;
        glm::vec2 ClipPlanes;
    public:
        Camera(gE::Entity* owner, glm::vec2 clipPlanes) : Component(owner), Projection(1.0), ClipPlanes(clipPlanes) {}

        void OnLoad() override { OnUpdate(0); UpdateProjection(); }
        void OnUpdate(double delta) final { }
        void OnDestroy() final { }

        [[nodiscard]] const glm::mat4* GetProjection() const { return &Projection; }
        [[nodiscard]] glm::mat4 GetView() const;
        [[nodiscard]] glm::vec2 GetClipPlanes() const { return ClipPlanes; }

        virtual void Use();
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
        glm::vec4 Info;
        glm::vec3 Position;
    };

    class CameraManager final : public ComponentManager<Camera>
    {
    private:
        Buffer<CameraData> p_CameraBuffer;
        Camera* p_CurrentCamera;
        Window* p_Window;
        Math::Frustum p_ViewFrustum;
    public:
        explicit CameraManager(Window* window) : ComponentManager<Camera>(), p_CurrentCamera(nullptr),
                                        p_CameraBuffer(window), p_Window(window), p_ViewFrustum(glm::mat4(1))
        {
            p_CameraBuffer.Bind(0, BufferTarget::UNIFORM);
        }

        void SetCamera(Camera* camera) { p_CurrentCamera = camera; }
        Camera* GetCamera() { return p_CurrentCamera; }

        glm::mat4 GetView() const;
        [[nodiscard]] Math::Frustum const* GetFrustum() const { return &p_ViewFrustum; }

        void OnUpdate(double delta) override;
    };
}
