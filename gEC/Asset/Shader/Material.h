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
    private:
        const Shader* p_Shader;
        const Shader* p_DepthShader;
    public:
        Material(Window* window, const Shader* shader, const Shader* depthShader = nullptr) : Asset(window), p_Shader(shader),
                                                                                            p_DepthShader(depthShader) {}

        void Use();

        virtual void MandatorySetup() = 0;
        virtual void RenderStageSetup() = 0;
    };
} // Asset
