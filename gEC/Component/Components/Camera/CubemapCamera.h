//
// Created by scion on 1/31/2023.
//

#pragma once

#include "Camera.h"
#include "../../../Asset/Buffer/Framebuffer.h"
#include "../../../Asset/Buffer/Renderbuffer.h"
#include "../../../Asset/Texture/TextureCube.h"
#include "../../../Asset/Buffer/Buffer.h"
#include "../../../Asset/VAO/VAO.h"
#include "../../../Asset/Shader/Shader.h"
#include "../../../Windowing/Stage.h"

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
        CubemapData(TextureHandle handle, const glm::vec3& center, const glm::vec3& extents) : CubemapHandle(handle), Center(center), Extent(extents) {}

        TextureHandle CubemapHandle;
        alignas(16) glm::vec3 Center;
        alignas(16) glm::vec3 Extent;
    };

    struct CubemapBufferData
    {
        TextureHandle SkyboxID;
        CubemapData Cubemaps[CMMANAGER_MAX_CUBEMAPS];
    };

    struct SkyboxInfo
    {
        const Asset::VAO *const SkyboxVAO;
        const Asset::Shader SkyboxShader;
        Asset::Texture* SkyboxTexture;

        SkyboxInfo(gE::Window* window, Asset::Texture* tex) : SkyboxVAO(Utility::CreateSkyboxVAO(window)), SkyboxShader(window, "../res/shader/skybox.vert", "../res/shader/skybox.frag", Asset::CullMode::NEVER, Asset::DepthFunction::LEQUAL), SkyboxTexture(tex){}
        ~SkyboxInfo() { delete SkyboxVAO; }

        void Render() const;
    };

    class CubemapManager final : public ComponentManager<CubemapCamera>
    {
    private:
        Asset::Shader _convolutionShader;
        Asset::Framebuffer _convolutionBuffer;
    public:
        CubemapManager(Window* window);

        SkyboxInfo Skybox;
        Buffer<CubemapBufferData> CubemapBuffer;

        void UpdateSkybox(Asset::Texture* tex);
        void Convolute(Asset::Texture* src, Asset::Texture* dst);

        void OnUpdate(double delta) override;
    };
}
