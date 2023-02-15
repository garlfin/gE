//
// Created by scion on 1/31/2023.
//

#pragma once

#include "Camera.h"
#include "../../../Asset/Buffer/Framebuffer.h"
#include "../../../Asset/Buffer/Renderbuffer.h"
#include "../../../Asset/Texture/TextureCube.h"
#include "../../../Asset/Buffer/Buffer.h"

#define CMMANAGER_MAX_CUBEMAPS 1

namespace gE::Component
{
    class CubemapCamera final : public Camera
    {
    public:
        CubemapCamera(Entity* owner, uint32_t size, glm::vec2 clipPlanes);
        glm::mat4 GetView() const override;

    protected:
        void UpdateProjection() override;
        void RenderPass(double delta) override;
    };


    struct CubemapData
    {
        TextureHandle CubemapHandle;
        alignas(16) glm::vec3 Center;
        alignas(16) glm::vec3 Extent;
    };

    class CubemapManager final : ComponentManager<CubemapCamera>
    {
    private:
        Buffer<CubemapData>* CubemapBuffer;
    public:
        CubemapManager(Window* window);
    };
}
