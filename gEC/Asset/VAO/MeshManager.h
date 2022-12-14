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

#define MAX_INSTANCE_COUNT 100u

namespace gE::Asset
{
    class MeshManager final : public AssetManager<std::pair<RenderMesh*, std::vector<Entity*>>>
    {
    private:
        struct ObjectInfo
        {
            glm::mat4 Model[MAX_INSTANCE_COUNT];
            glm::mat4 NormalMatrix[MAX_INSTANCE_COUNT];
            uint32_t ObjectCount;
        };

        typedef std::pair<RenderMesh*, std::vector<Entity*>> Pair;
        typedef AssetManager<Pair> Base;
        Buffer<ObjectInfo> p_ModelBuffer;
        Window* p_Window;

    protected:
        void Destruct(Pair *t) override;

    public:
        explicit MeshManager(Window* window) : Base(), p_Window(window), p_ModelBuffer(window) { p_ModelBuffer.Bind(1, BufferTarget::UNIFORM); }

        Pair* Register(RenderMesh* mesh)
        {
            return Create<Pair>(mesh, std::vector<Entity*>());
        };

        Pair* FindPair(RenderMesh* mesh);
        void Register(Component::Renderer*);
        void Remove(Component::Renderer*);

        void OnRender();

    };
}
