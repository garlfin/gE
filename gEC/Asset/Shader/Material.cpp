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

        if(GetWindow()->GetStage() == Windowing::Stage::PreZ)
            return p_DepthShader->Use();
        if(GetWindow()->GetStage() == Windowing::Stage::Shadow)
            return GetWindow()->GetDefaultShader()->Use(p_Shader->GetDepthFunc(), p_Shader->GetCullMode());


        p_Shader->Use();
        RenderStageSetup();
    }

    Material::Material(Window* window, const Shader* shader, const Shader* depthShader) : Asset(window), p_Shader(shader),
                                                                                          p_DepthShader(depthShader), p_DepthShaderOwned(false)
    {
        if(depthShader) return;

        p_DepthShaderOwned = true;
        p_DepthShader = new Shader(GetWindow(), shader->GetVertStage(), GetWindow()->GetDefaultFragment(), shader->GetCullMode(), shader->GetDepthFunc());
    }

    void DeferredMaterial::Use()
    {
        MandatorySetup();
        if(GetWindow()->GetStage() & Windowing::Stage::PreZ)
            return p_DepthShader->Use();
        else if(GetWindow()->GetStage() & Windowing::Stage::Shadow)
            return GetWindow()->GetDefaultShader()->Use(p_Shader->GetDepthFunc(), p_Shader->GetCullMode());
        else
            (GetWindow()->GetStage() == Windowing::Stage::Cubemap ? _forwardShader : p_Shader)->Use();
        RenderStageSetup();
    }
}