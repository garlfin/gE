//
// Created by scion on 11/17/2022.
//

#include "VAO.h"
#include <memory>
#include "GLAD/glad.h"

namespace gE::Asset
{
    void VAO::Use()
    {
        glBindVertexArray(ID);
    }

    VAO::~VAO()
    {
        glDeleteVertexArrays(1, &ID);
    }

    void VAO::Draw(uint32_t instanceCount)
    {
        Bind();
        if (instanceCount - 1)
            glDrawArraysInstanced(GL_TRIANGLES, 0, TriCount * 3, instanceCount);
        else
            glDrawArrays(GL_TRIANGLES, 0, TriCount * 3);
    }

    void VAO::Bind()
    {
        glBindVertexArray(ID);
    }

    VAO::VAO(Window* window, FieldInfo fields, uint32_t triCount, void* data) : GLAsset::GLAsset(window), VBO(window, triCount, sizeof(glm::vec3) + fields.CalculateSize(), (uint8_t*) data), TriCount(triCount)
    {
        glCreateVertexArrays(1, &ID);

        glVertexArrayVertexBuffer(ID, 0, VBO.Get(), 0, sizeof(glm::vec3) + fields.CalculateSize());

        glEnableVertexArrayAttrib(ID, 0);
        glVertexArrayAttribFormat(ID, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(ID, 0, 0);

        uint8_t currentOffset = sizeof(glm::vec3);
        for(uint8_t i = 1; i < 5; i++)
        {
            if (!fields.Values[i]) continue;

            glEnableVertexArrayAttrib(ID, i);
            glVertexArrayAttribFormat(ID, i, i == 1 ? 2 : 3, GL_FLOAT, GL_FALSE, currentOffset);
            glVertexArrayAttribBinding(ID, i, 0);

            currentOffset += FieldInfo::GetFieldSize(i);
        }
    }
}

gE::Asset::VAO *gE::Utility::CreateSkyboxVAO(gE::Window *window)
{
    static const float Vertices[108]
            {
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,

                    -0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,

                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,

                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,

                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f, -0.5f, -0.5f,

                    -0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
            };

    return new Asset::VAO(window, FieldInfo(false, false, false, false), 12, (void*) Vertices);
}
