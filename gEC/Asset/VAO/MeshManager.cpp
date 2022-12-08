//
// Created by scion on 12/8/2022.
//

#include "MeshManager.h"

namespace gE::Asset
{
    void MeshManager::Destruct(std::pair<VAO*, std::vector<Entity*>> *t)
    {
        delete t->first;
        delete t;
    }

    void MeshManager::Register(Component::Renderer *renderer)
    {
        Pair* pair = FindPair(renderer->GetRenderMesh());

        if (!pair)
            pair = Register(renderer->GetRenderMesh());

        pair->second.push_back(renderer->GetOwner());
    }

    MeshManager::Pair *MeshManager::FindPair(VAO *vao)
    {
        for (auto* asset : Base::p_Assets)
            if (asset->first == vao)
                return asset;

        return nullptr;
    }

    void MeshManager::Remove(Component::Renderer *renderer)
    {
        Pair* pair = FindPair(renderer->GetRenderMesh());
        if(pair) std::erase(pair->second, renderer->GetOwner());
    }
}