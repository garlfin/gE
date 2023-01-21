//
// Created by scion on 12/8/2022.
//

#include "MeshManager.h"
#include "../../Component/Components/Transform.h"
#include "glm/gtc/matrix_inverse.hpp"
#include "../../Windowing/Window.h"
#include "../Shader/Material.h"
#include "../../Component/Components/MaterialHolder.h"
#include "glm/ext/matrix_transform.hpp"

void DrawSubMesh(gE::Window* window, const gE::Entity* e, const gE::Asset::VAO* subMesh, uint32_t count, uint32_t index);
void SortVisibleEntities(const gE::Entity** entities, uint32_t count, gE::Window* window, gE::Asset::RenderMesh* mesh);

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
        for(Pair *pair: Base::p_Assets)
        {
            if(!pair->first) continue;

            uint32_t count = 0;
            for (uint32_t i = 0; i < pair->second.size(); i++)
                if (pair->second[i]->GetComponent<Component::Renderer>()->IsInView)
                {
                    p_EligibleEntities[count] = pair->second[i];
                    count++;
                }

            if(!count) continue;
            for(uint32_t i = 0; i < count; i += MAX_INSTANCE_COUNT)
            {
                uint32_t drawCount = std::min((unsigned) count - i, MAX_INSTANCE_COUNT);
                p_Window->TransformManager->UpdateMatrices(&p_EligibleEntities[i], drawCount);

                for(uint8_t x = 0; x < pair->first->Mesh->SubMeshCount; x++)
                    DrawSubMesh(p_Window, pair->second[0], pair->first->Renderers[x], drawCount, x);
            }
        }
    }

    void MeshManager::OnUpdate(double delta)
    {
        uint32_t count = 0;
        for(Pair *pair : Base::p_Assets)
            count = std::max(count, (uint32_t) pair->second.size());

        if(p_PreviousCount != count)
        {
            delete[] p_EligibleEntities;
            p_EligibleEntities = new const Entity*[count];
        }
        p_PreviousCount = count;

        for(Pair *pair: Base::p_Assets)
        {
            memcpy(p_EligibleEntities, pair->second.data(), pair->second.size() * sizeof(DynamicEntity*));
            count = pair->second.size();

            SortVisibleEntities(p_EligibleEntities, count, p_Window, pair->first);
        }
    }
}

void SortVisibleEntities(const gE::Entity** entities, uint32_t count, gE::Window* window, gE::Asset::RenderMesh* mesh)
{
    for(uint32_t i = 0; i < count; i++)
    {
        auto* renderer = entities[i]->GetComponent<gE::Component::Renderer>();
        auto* transform = entities[i]->GetComponent<gE::Component::Transform>();
        renderer->IsInView = true;

        //if(window->CameraManager->GetFrustum()->Collide(gE::Math::AABB(mesh->Mesh->Bounds, transform->Model))) renderer->IsInView = true;
    }
}

void DrawSubMesh(gE::Window* window, const gE::Entity* e, const gE::Asset::VAO* subMesh, uint32_t count, uint32_t i)
{
    gE::Asset::Material* renderMaterial = nullptr;

    if(auto* mHolder = e->GetComponent<gE::Component::MaterialHolder>())
        renderMaterial = mHolder->GetMaterial(i);

    if (renderMaterial != nullptr) renderMaterial->Use();
    else window->GetDefaultShader()->Use();

    subMesh->Draw(count);
}
