//
// Created by scion on 12/8/2022.
//

#pragma once

#include "../AssetManager.h"
#include "../../Component/Entity.h"
#include "VAO.h"
#include "../../Component/Components/Renderer.h"

namespace gE::Asset
{
    class MeshManager final : public AssetManager<std::pair<VAO*, std::vector<Entity*>>>
    {
    private:
        typedef std::pair<VAO*, std::vector<Entity*>> Pair;
        typedef AssetManager<Pair> Base;
    protected:
        void Destruct(std::pair<VAO*, std::vector<Entity*>> *t) override;

        Pair* Register(VAO* vao)
        {
            Create<Pair>(vao, std::vector<Entity*>());
        };

        template<typename I, typename... Args>
        I* CreateVAO(Args&& ... args)
        {
            return (I*) (Create<Pair>(new I(std::forward<Args>(args)...), std::vector<Entity*>()).first);
        }

        Pair* FindPair(VAO*);

        void Register(Component::Renderer*);
        void Remove(Component::Renderer*);
    };
}
