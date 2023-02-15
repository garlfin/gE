//
// Created by scion on 12/12/2022.
//

#include "Material.h"
#include "../../Windowing/Window.h"

namespace gE::Asset
{
    void Material::Use()
    {
        MandatorySetup();

        if(GetWindow()->GetStage() & (Windowing::Stage::PreZ | Windowing::Stage::Shadow))
            return (p_DepthShader ?: GetWindow()->GetDefaultShader())->Use(p_Shader->GetDepthFunc(), p_Shader->GetCullMode());

        p_Shader->Use();
        RenderStageSetup();
    }

    void DeferredMaterial::Use()
    {
        MandatorySetup();
        if(GetWindow()->GetStage() & (Windowing::Stage::PreZ | Windowing::Stage::Shadow))
            return (p_DepthShader ?: GetWindow()->GetDefaultShader())->Use(p_Shader->GetDepthFunc(), p_Shader->GetCullMode());
        else
            (GetWindow()->GetStage() == Windowing::Stage::Cubemap ? _forwardShader : p_Shader)->Use();
        RenderStageSetup();
    }
}