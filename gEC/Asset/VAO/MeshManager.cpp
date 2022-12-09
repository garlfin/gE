//
// Created by scion on 12/8/2022.
//

#include "MeshManager.h"
#include "../../Component/Components/Transform.h"

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

    void MeshManager::OnRender()
    {
        for (Pair *pair: Base::p_Assets)
            for (uint32_t i = 0; i < pair->second.size(); i += MAX_INSTANCE_COUNT)
            {
                static ObjectInfo info;
                info.ObjectCount = std::min((unsigned) pair->second.size(), MAX_INSTANCE_COUNT);
                for(uint32_t x = 0; x < info.ObjectCount; x++)
                    info.Model[x] = pair->second[i * MAX_INSTANCE_COUNT + x]->GetComponent<Component::Transform>()->Model;

                ModelBuffer.ReplaceData(&info);
                pair->first->Draw(info.ObjectCount);
            }
    }
}