//
// Created by scion on 1/19/2023.
//

#pragma once

#include "../gEC/Component/Components/Behavior.h"
#include "../gEC/Component/Components/Transform.h"
#include "Weapon.h"

namespace gE::Component
{
    class InventoryScript final : public Behavior
    {
    private:
        float _lerpValue = 0, _swayValue = 0;
        Transform* _transform;

    public:
        InventoryScript(Entity* owner) : Behavior(owner) {}

        void OnLoad() override;
        void OnUpdate(double delta) override;

        gE::Weapon* Weapon;
    };
}
