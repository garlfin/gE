//
// Created by scion on 12/8/2022.
//

#pragma once

#include "../AssetManager.h"
#include "../../Component/Entity.h"
#include "VAO.h"
#include "../../Component/Components/Renderer.h"
#include "../Buffer/Buffer.h"
#include "glm/mat4x4.hpp"

#define MAX_INSTANCE_COUNT 100u

namespace gE::Asset
{
    class MeshManager final : public AssetManager<std::pair<VAO*, std::vector<Entity*>>>
    {
    private:
        struct ObjectInfo
        {
            glm::mat4 Model[MAX_INSTANCE_COUNT];
            glm::mat3 NormalMatrix[MAX_INSTANCE_COUNT];
            uint32_t ObjectCount;
        };

        typedef std::pair<VAO*, std::vector<Entity*>> Pair;
        typedef AssetManager<Pair> Base;
        Buffer<ObjectInfo> p_ModelBuffer;
        Window* p_Window;

    protected:
        void Destruct(std::pair<VAO*, std::vector<Entity*>> *t) override;

    public:
        explicit MeshManager(Window* window) : Base(), p_Window(window), p_ModelBuffer(window) { p_ModelBuffer.Bind(1, BufferTarget::UNIFORM); }

        Pair* Register(VAO* vao)
        {
            return Create<Pair>(vao, std::vector<Entity*>());
        };

        template<typename I, typename... Args>
        I* CreateVAO(Args&& ... args)
        {
            return (I*) (Create<Pair>(new I(std::forward<Args>(args)...), std::vector<Entity*>()).first);
        }

        Pair* FindPair(VAO*);

        void Register(Component::Renderer*);
        void Remove(Component::Renderer*);

        void OnRender();
    };
}
