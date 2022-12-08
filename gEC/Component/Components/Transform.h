//
// Created by scion on 11/10/2022.
//

#pragma once

#include "../Component.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace gE::Component
{
    class Transform final : public Component::Component
    {
    public:
        Transform(Entity* owner) : Component::Component(owner), Location(0.0), Rotation(0.0), Scale(1.0) {};

        glm::vec3 Location;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        glm::mat4 Model;

        void OnLoad() override;
        void OnRender(double delta) override;
        void OnUpdate(double delta) override;
        void OnDestroy() override {}
    };
}
