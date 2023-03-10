//
// Created by scion on 11/10/2022.
//

#pragma once

#include "../Component.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "../../Asset/Buffer/Buffer.h"
#include "gEModel.h"

#define MAX_INSTANCE_COUNT 100u

namespace gE::Component
{
    class Transform final : public Component::Component
    {
    public:
        Transform(Entity* owner) : Component::Component(owner), Location(0.0), Rotation(0.0), Scale(1.0) {};
        Transform(Entity* owner, gE::Transform const& t) : Component::Component(owner), Location(t.Location), Rotation(t.Rotation), Scale(t.Scale) {};

        glm::vec3 Location;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        glm::mat4 Model;
        glm::mat4 PreviousModel;

        void OnLoad() override;
        void OnRender(double delta) override;
        void OnUpdate(double delta) override;
        void OnDestroy() override {}

        void Set(const gE::Transform& t)
        {
            Location = t.Location;
            Rotation = t.Rotation;
            Scale = t.Scale;
        }
    };

    struct ObjectInfo
    {
        glm::mat4 Model[MAX_INSTANCE_COUNT];
        glm::mat4 PreviousModel[MAX_INSTANCE_COUNT];
        glm::mat4 NormalMatrix[MAX_INSTANCE_COUNT];
        uint32_t ObjectCount;
    };

    class TransformManager final : public ComponentManager<Transform>
    {
    private:
        Buffer<ObjectInfo> p_Buffer;
        ObjectInfo p_Info{};

    public:
        explicit TransformManager(gE::Window* window) : ComponentManager<Transform>(nullptr), p_Buffer(window)
        {
            p_Buffer.Bind(1, BufferTarget::UNIFORM);
        }
        void UpdateMatrices(const Entity** entities, uint32_t count);
        Buffer<ObjectInfo>* GetBuffer() { return &p_Buffer; }
    };
}
