//
// Created by scion on 12/14/2022.
//

#pragma once


#include "../../gEC/Asset/Shader/Material.h"
#include "../../gEC/Windowing/DemoWindow.h"

namespace gE::Asset
{
    class PBRMaterial final : public Material
    {
    public:
        PBRMaterial(gE::DemoWindow* pWindow, Shader* pShader);

        void MandatorySetup() override;
        void RenderStageSetup() override;
    };
}
