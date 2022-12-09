//
// Created by scion on 12/2/2022.
//

#include "Renderer.h"
#include "../../Asset/VAO/VAO.h"
#include "../../Windowing/Window.h"

namespace gE::Component
{
    Renderer::Renderer(Entity* owner, Asset::VAO* mesh) : Component::Component(owner, false),  p_renderMesh(mesh)
    {
        GetWindow()->MeshManager->Register(this);
    }

    void Renderer::OnDestroy()
    {
        GetWindow()->MeshManager->Remove(this);
    }
}