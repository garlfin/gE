//
// Created by scion on 12/12/2022.
//

#include "Material.h"
#include "../../Windowing/Window.h"

namespace gE::Asset
{
    void Material::Use()
    {
        if(GetWindow()->GetStage() == Windowing::Stage::PreZ)
            (p_DepthShader ?: GetWindow()->GetDefaultShader())->Use();
        MandatorySetup();

        if(GetWindow()->GetStage() != Windowing::Stage::Render) return;
        p_Shader->Use();
        RenderStageSetup();
    }
}