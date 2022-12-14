//
// Created by scion on 11/17/2022.
//

#include "VAO.h"
#include <memory>
#include <iostream>
#include "GLAD/glad.h"
#include "IndexedVAO.h"

namespace gE::Asset
{
    void VAO::Use() const
    {
        glBindVertexArray(ID);
    }

    VAO::~VAO()
    {
        glDeleteVertexArrays(1, &ID);
    }

    void VAO::Draw(uint32_t instanceCount) const
    {
        Bind();
        if (instanceCount - 1)
            glDrawArraysInstanced(GL_TRIANGLES, 0, TriCount * 3, instanceCount);
        else
            glDrawArrays(GL_TRIANGLES, 0, TriCount * 3);
    }

    void VAO::Bind() const
    {
        glBindVertexArray(ID);
    }

    VAO::VAO(Window* window, FieldInfo fields, uint32_t triCount, void* data) : GLAsset::GLAsset(window), VBO(window, triCount, sizeof(glm::vec3) + fields.CalculateSize(), (uint8_t*) data), TriCount(triCount)
    {
        glCreateVertexArrays(1, &ID);

        glVertexArrayVertexBuffer(ID, 0, VBO.Get(), 0, sizeof(glm::vec3) + fields.CalculateSize());


        uint8_t currentOffset = 0;
        for(uint8_t i = 0; i < 5; i++, currentOffset += FieldInfo::GetFieldSize(i))
        {
            if(i && !fields.Values[i - 1])
                continue;
            //std::cout << (uint32_t) i << std::endl;
            glEnableVertexArrayAttrib(ID, i);
            glVertexArrayAttribFormat(ID, i, i == 1 ? 2 : 3, GL_FLOAT, GL_FALSE, currentOffset);
            glVertexArrayAttribBinding(ID, i, 0);
        }
    }
}

gE::Asset::VAO *gE::Utility::CreateSkyboxVAO(gE::Window *window)
{
    static const float Vertices[]
            {
                -1, -1, -1,
                1, -1, -1,
                -1, 1, -1,
                -1, 1, -1,

                -1, -1, 1,
                1, -1, 1,
                1, 1, 1,
                -1, 1, 1
            };
    static const uint32_t Faces[]
            {
                0, 1, 2,
                2, 3, 0,
                4, 5, 6,
                6, 7, 4,
                0, 3, 7,
                7, 4, 0,
                1, 2, 6,
                6, 5, 1,
                3, 7, 6,
                6, 2, 3,
                0, 4, 5,
                5, 1, 0
            };

    return new Asset::IndexedVAO(window, FieldInfo(false, false, false, false), 8, 12, (void*) &Vertices, (void*) &Faces);
}
