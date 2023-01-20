//
// Created by scion on 1/17/2023.
//

#pragma once

#include "../gEC/Component/Entity.h"
#include "../gEC/Component/Components/Transform.h"
#include "../gEC/Component/Components/Renderer.h"
#include "../gEC/Component/Components/MaterialHolder.h"
#include "gEModel.h"

namespace gE
{
    class StaticRenderer : public StaticEntity<Component::Transform, Component::Renderer, Component::MaterialHolder>
    {
    public:
        StaticRenderer(Window* window, const Transform& transform, Asset::RenderMesh* mesh, Asset::Material** materials,
                       uint32_t matCount);
    };
}