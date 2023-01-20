//
// Created by scion on 1/18/2023.
//

#pragma once

#include "../gEC/Asset/VAO/RenderMesh.h"
#include "../../gEC/Component/Components/Transform.h"
#include "../../gEC/Asset/Shader/Material.h"

#define WPN_MAX_MATERIALS 4

namespace gE
{
    struct Weapon
    {
        Weapon(Asset::RenderMesh* mesh, Asset::Material** materials, uint8_t matCount, const Transform& idle, const Transform& aim, float adsTime)
        : Mesh(mesh), MatCount(matCount), IdleOffset(idle), AimOffset(aim), ADSTime(adsTime) { memcpy(Materials, materials, matCount); }

        Asset::RenderMesh* Mesh;
        Asset::Material* Materials[WPN_MAX_MATERIALS];
        uint8_t MatCount;

        Transform IdleOffset;
        Transform AimOffset;

        float ADSTime;
    };
}
