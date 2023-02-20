//
// Created by scion on 12/12/2022.
//

#pragma once

#include "../GLAsset.h"
#include "Shader.h"

namespace gE::Asset
{
    class Material : public Asset
    {
    protected:
        const Shader* p_DepthShader;
        const Shader* p_Shader;
        bool p_DepthShaderOwned;
    public:
        Material(Window* window, const Shader* shader, const Shader* depthShader = nullptr);

        virtual void Use();

        virtual void MandatorySetup() = 0;
        virtual void RenderStageSetup() = 0;

        ~Material() { if(p_DepthShaderOwned) delete p_DepthShader; }
    };

    class DeferredMaterial : public Material
    {
    protected:
        const Shader* _forwardShader;
    public:
        DeferredMaterial(Window* window, const Shader* shader, const Shader* forwardShader, const Shader* depthShader = nullptr)
        : Material(window, shader, depthShader), _forwardShader(forwardShader) {};
        void Use() override;
    };
} // Asset


