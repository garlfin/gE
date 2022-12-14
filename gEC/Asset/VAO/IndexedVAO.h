//
// Created by scion on 11/22/2022.
//

#pragma once

#include "VAO.h"

namespace gE::Asset
{
class IndexedVAO final : public VAO
    {
    private:
        Buffer<glm::u32vec3> p_EBO;
    public:
        IndexedVAO(Window* window, SubMesh* mesh);
        IndexedVAO(Window* window, FieldInfo fields, uint32_t vCount, uint32_t triCount, void* vertices, void* tris);

        void Draw(uint32_t instanceCount) const override;
    };
}
