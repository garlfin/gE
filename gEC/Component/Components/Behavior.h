//
// Created by scion on 12/5/2022.
//

#pragma once

#include "../Component.h"

namespace gE::Component
{
    class Behavior : public Component
    {
    public:
        Behavior(Entity* owner) : Component(owner) {}
        void OnLoad() override {}
        void OnRender(double delta) override {}
        void OnUpdate(double delta) override {}
        void OnDestroy() override {}
    };
}