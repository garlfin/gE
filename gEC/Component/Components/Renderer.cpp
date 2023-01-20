//
// Created by scion on 12/2/2022.
//

#include "Renderer.h"
#include "../../Windowing/Window.h"

namespace gE::Component
{
    Renderer::Renderer(Entity* owner, Asset::RenderMesh* mesh) : Component::Component(owner), p_renderMesh(mesh)
    {
        GetWindow()->MeshManager->Register(this);
    }

    void Renderer::OnDestroy()
    {
        GetWindow()->MeshManager->Remove(this);
    }

    void Renderer::SetRenderMesh(Asset::RenderMesh* mesh)
    {
        p_renderMesh = mesh;
        
        GetWindow()->MeshManager->Remove(this);
        GetWindow()->MeshManager->Register(this);
    }
}