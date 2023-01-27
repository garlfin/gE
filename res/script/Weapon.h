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
    class Attachment
    {
    public:
        Attachment(Asset::RenderMesh* mesh, Asset::Material* material) : Mesh(mesh), Material(material) {}

        Asset::RenderMesh* Mesh;
        Asset::Material* Material;
    };

    class SightAttachment : public Attachment
    {
    public:
        SightAttachment(Asset::RenderMesh* mesh, Asset::Material* material,
                        const Transform& adsPoint, float fov) : Attachment(mesh, material), ADSPoint(adsPoint), ADSFov(fov)
        {

        }

        Transform ADSPoint;
        float ADSFov;
    };

    enum FireSelectMode
    {
        Single = 0b000,
        Burst2 = 0b100,
        Burst3 = 0b010,
        Auto = 0b001
    };

    class Weapon
    {
    public:
        Weapon(float baseAdsSpeed, FireSelectMode fireModes, const Transform& idleOffset, const Transform& sightPoint, SightAttachment* sight,
               Asset::RenderMesh* mesh, Asset::Material** materials, uint8_t matCount) : BaseADSSpeed(baseAdsSpeed),
                                                                                         FireModes(fireModes),
                                                                                         IdleOffset(idleOffset),
                                                                                         Sight(sight), Mesh(mesh),
                                                                                         MatCount(matCount), SightAttachmentPoint(sightPoint)
        {
            memcpy(Materials, materials, std::min((uint8_t) WPN_MAX_MATERIALS, matCount) * sizeof(Asset::Material*));
        }

        virtual void* Cast(Entity*, uint8_t* hitCount) { return nullptr; }

        float BaseADSSpeed;
        FireSelectMode FireModes;
        Transform IdleOffset;

        Transform SightAttachmentPoint;
        SightAttachment* Sight;

        Asset::RenderMesh* Mesh;
        Asset::Material* Materials[WPN_MAX_MATERIALS];
        uint8_t MatCount;
    };
}
