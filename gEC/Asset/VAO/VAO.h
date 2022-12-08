//
// Created by scion on 11/17/2022.
//

#pragma once
#include "../GLAsset.h"
#include "Mesh.h"
#include "../Buffer/Buffer.h"

namespace gE::Asset
{
    class VAO : public GLAsset
    {
    protected:
        Buffer<uint8_t> VBO;
        uint32_t TriCount;
    public:
        VAO(Window* window, FieldInfo fields, uint32_t triCount, void* data = nullptr);

        void Use();
        void Bind();

        virtual void Draw(uint32_t instanceCount);

        ~VAO() override;
    };
}
