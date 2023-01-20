//
// Created by scion on 12/2/2022.
//

#pragma once

#include "../Component.h"
#include "../../Asset/VAO/RenderMesh.h"

namespace gE::Asset { class VAO; }

namespace gE::Component
{
    class Renderer final : public Component::Component
    {
    private:
        Asset::RenderMesh* p_renderMesh;
    public:
        Renderer(Entity* owner, Asset::RenderMesh* mesh);

        void OnLoad() override {};
        void OnRender(double delta) override {};
        void OnUpdate(double delta) override {};
        void OnDestroy() override;

        bool IsInView;

        Asset::RenderMesh* GetRenderMesh() const { return p_renderMesh; }
        void SetRenderMesh(Asset::RenderMesh* mesh);
        const Asset::VAO* const GetVAO(uint32_t i) const { return p_renderMesh->Renderers[i]; }
    };
}
