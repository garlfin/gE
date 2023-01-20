//
// Created by scion on 1/17/2023.
//

#include "StaticRenderer.h"
#include "../gEC/Windowing/Window.h"

namespace gE
{
    StaticRenderer::StaticRenderer(Window* window, const Transform& transform, Asset::RenderMesh* mesh, Asset::Material** materials, uint32_t matCount) : Base::Base(window)
    {
        Base::Components[0] = (Component::Component*) window->TransformManager->Create<Component::Transform>((Entity*) this, transform);
        Base::Components[1] = (Component::Component*) window->ComponentManager.Create<Component::Renderer>((Entity*) this, mesh);
        Base::Components[2] = (Component::Component*) window->ComponentManager.Create<Component::MaterialHolder>((Entity*) this, materials, matCount);
    }
}


