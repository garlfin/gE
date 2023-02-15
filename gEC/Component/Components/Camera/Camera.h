//
// Created by scion on 11/30/2022.
//

#pragma once

#include "../../Component.h"
#include "../../../Asset/Buffer/Buffer.h"
#include "../../../Utility/Frustum.h"
#include "glm/mat4x4.hpp"
#include "../../../Asset/Buffer/Framebuffer.h"
#include "../../../Asset/Texture/Texture.h"

namespace gE::Component
{
    enum class CameraFields : uint8_t
    {
        DEPTH            = 0b00000000,
        COLOR            = 0b00000001,
        DEPTH_COPY       = 0b00000010,
        COLOR_COPY       = 0b00000100,
        CUBEMAP          = 0b00001000,
        SCREEN_SPACE_RDY = COLOR | DEPTH_COPY | COLOR_COPY,
        CUBEMAP_RDY      = COLOR | CUBEMAP
    };

    class Camera : public Component
    {
    protected:
        Camera(gE::Entity* owner, glm::vec2 clipPlanes, glm::uvec2 dimensions, CameraFields fields, Asset::TextureType colorFormat);

        virtual void RenderPass(double delta) = 0;
        virtual void UpdateProjection() = 0;

        glm::mat4 Projection;
        glm::vec2 ClipPlanes;

        Asset::Framebuffer* const Framebuffer;
        Asset::Texture* InternalDepth, *InternalColor, *Color, *Depth;

    public:
        void OnRender(double delta) final;
        void OnLoad() override { OnUpdate(0); UpdateProjection(); }
        void OnUpdate(double delta) final { }
        void OnDestroy() final { }

        [[nodiscard]] const glm::mat4& GetProjection() const { return Projection; }
        [[nodiscard]] virtual glm::mat4 GetView() const;
        [[nodiscard]] glm::vec2 GetClipPlanes() const { return ClipPlanes; }

        Asset::Texture* GetColor() { return Color ?: InternalColor; }
        Asset::Texture* GetDepth() { return Depth ?: InternalDepth; }
        glm::uvec2 GetSize() { return GetDepth()->GetSize(); }

        void Use();

        ~Camera() override;
    };

    struct CameraData
    {
    public:
        CameraData(const glm::mat4& view, const glm::mat4 & projection, const glm::vec3& position, const glm::vec4& info, uint64_t color, uint64_t depth)
                : View(view), Projection(projection), Position(position), Info(info), Color(color), Depth(depth)
        {}

        glm::mat4 View;
        glm::mat4 Projection;
        glm::vec4 Info;
        glm::vec3 Position;

        uint64_t Color;
        uint64_t Depth;
    };

    class CameraManager final : public ComponentManager<Camera>
    {
    private:
        Buffer<CameraData> p_CameraBuffer;
        Camera* p_CurrentCamera;
    public:
        explicit CameraManager(Window* window) : ComponentManager<Camera>(window), p_CurrentCamera(nullptr),
                                                 p_CameraBuffer(window)
        {
            p_CameraBuffer.Bind(0, BufferTarget::UNIFORM);
        }

        void SetCamera(Camera* camera) { p_CurrentCamera = camera; }
        Camera* GetCamera() { return p_CurrentCamera; }
        Buffer<CameraData>* GetBuffer() { return &p_CameraBuffer; }

        glm::mat4 GetView() const;
    };

    inline uint8_t operator&(CameraFields a, CameraFields b)
    {
        return ((uint8_t)(a) & (std::uint8_t)(b));
    }
}
