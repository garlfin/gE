//
// Created by scion on 12/8/2022.
//

#pragma once

#include "../AssetManager.h"
#include "../../Component/Entity.h"
#include "RenderMesh.h"
#include "../../Component/Components/Renderer.h"
#include "../Buffer/Buffer.h"
#include "glm/mat4x4.hpp"

namespace gE::Asset
{
    class MeshManager final : public AssetManager<std::pair<RenderMesh*, std::vector<Entity*>>>
    {
    private:
        typedef std::pair<RenderMesh*, std::vector<Entity*>> Pair;
        typedef AssetManager<Pair> Base;
        Window* p_Window;
        const Entity** p_EligibleEntities;
        uint32_t p_PreviousCount;

    protected:
        void Destruct(Pair *t) override;

    public:
        explicit MeshManager(Window* window) : Base(), p_Window(window), p_EligibleEntities(nullptr)
                                                    , p_PreviousCount(0) {}

        Pair* Register(RenderMesh* mesh)
        {
            return Create<Pair>(mesh, std::vector<Entity*>());
        };

        Pair* FindPair(RenderMesh* mesh);
        void Register(Component::Renderer*);
        void Remove(Component::Renderer*);

        void OnUpdate(double delta) override;

        void OnRender();
    };
}
