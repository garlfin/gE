//
// Created by scion on 12/5/2022.
//

#pragma once

#include "../../gEC/Component/Components/Behavior.h"
#include "../../gEC/Component/Components/Transform.h"

namespace gE::Component
{
    class CameraMovement final : public Behavior
    {
    private:
        Transform* transform = nullptr;
        glm::dvec2 prevMousePos = glm::dvec2(0);
        glm::vec3 velocity = glm::vec3(0);

    public:
        explicit CameraMovement(Entity* owner) : Behavior(owner) {}
        void OnLoad() override;
        void OnUpdate(double delta) override;
    };
}
