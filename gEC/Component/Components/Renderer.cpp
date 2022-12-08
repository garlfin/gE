//
// Created by scion on 12/2/2022.
//

#include "Renderer.h"
#include "../../Asset/VAO/VAO.h"

namespace gE::Component
{
    Renderer::Renderer(Entity* owner, Asset::VAO* mesh) : Component::Component(owner),  p_renderMesh(mesh)
    {

    }

    void Renderer::OnRender(double delta)
    {
        p_renderMesh->Draw(1);
    }
}