//
// Created by scion on 12/12/2022.
//

#pragma once

#include <memory>
#include "../Component.h"
#include "../../Asset/Shader/Material.h"

#define MAX_MATERIALS 8

namespace gE::Component
{
    class MaterialHolder final : public Component
    {
    private:
        gE::Asset::Material* Materials[MAX_MATERIALS] {};

    public:
        MaterialHolder(Entity* owner, gE::Asset::Material** materials = nullptr, uint32_t count = 0) : Component(owner)
        {
            if(materials) memcpy(Materials, materials, count * sizeof(gE::Asset::Material*));
        };

        void SetMaterial(uint32_t index, gE::Asset::Material* mat) { Materials[index] = mat; }
        gE::Asset::Material* GetMaterial(uint32_t index) { return Materials[index]; }

        void OnLoad() override {};
        void OnRender(double delta) override {};
        void OnUpdate(double delta) override {};
        void OnDestroy() override {};
    };
}
