//
// Created by scion on 12/8/2022.
//

#include "MeshManager.h"
#include "../../Component/Components/Transform.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "../../Windowing/Window.h"
#include "../Shader/Material.h"
#include "../../Component/Components/MaterialHolder.h"

namespace gE::Asset
{
    void MeshManager::Destruct(Pair *t)
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

    MeshManager::Pair *MeshManager::FindPair(RenderMesh *mesh)
    {
        for (auto* asset : Base::p_Assets)
            if (asset->first == mesh)
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
                {
                    auto* transform = pair->second[i * MAX_INSTANCE_COUNT + x]->GetComponent<Component::Transform>();
                    info.Model[x] = transform->Model;
                    info.NormalMatrix[x] = glm::inverseTranspose(glm::mat3(transform->Model));
                }

                for (uint8_t y = 0; y < pair->first->Mesh->SubMeshCount; y++)
                {
                    Material* renderMaterial = nullptr;

                    if(auto* mHolder = pair->second[0]->GetComponent<Component::MaterialHolder>())
                        renderMaterial = mHolder->GetMaterial(0);

                    if (renderMaterial != nullptr) renderMaterial->Use();
                    else p_Window->GetDefaultShader()->Use();

                    p_ModelBuffer.ReplaceData(&info);
                    pair->first->Renderers[y]->Draw(info.ObjectCount);
                }
            }
    }
}