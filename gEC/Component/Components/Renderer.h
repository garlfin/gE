//
// Created by scion on 12/2/2022.
//

#pragma once

#include "../Component.h"

namespace gE::Asset { class VAO; }

namespace gE::Component
{
    class Renderer final : public Component::Component
    {
    private:
        Asset::VAO* p_renderMesh;
    public:
        Renderer(Entity* owner, Asset::VAO* mesh);

        void OnLoad() override {};
        void OnRender(double delta) override {};
        void OnUpdate(double delta) override {};
        void OnDestroy() override;

        Asset::VAO* GetRenderMesh() const { return p_renderMesh; }
    };
}
