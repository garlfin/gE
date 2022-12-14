//
// Created by scion on 11/22/2022.
//

#include "IndexedVAO.h"

namespace gE::Asset
{

    IndexedVAO::IndexedVAO(Window* window, SubMesh* mesh) : VAO(window, mesh->Fields, mesh->VertexCount),
                                                                p_EBO(window, mesh->FaceCount, 1, mesh->Indices)
    {
        uint8_t totalVertexOffset = sizeof(glm::vec3) + mesh->Fields.CalculateSize();
        uint8_t* byteData = new uint8_t[(sizeof(glm::vec3) + mesh->Fields.CalculateSize()) * mesh->VertexCount];

        // Mandatory position attribute
        for(uint8_t b = 0, offset = 0; b < 5; b++)
        {
            if(b && !mesh->Fields.Values[b - 1]) continue;
            for(uint32_t i = 0; i < mesh->VertexCount; i++)
                if(b - 1)
                    *(glm::vec3*)(byteData + offset + i * totalVertexOffset) = ((glm::vec3*) mesh->GetField(b))[i];
                else
                    *(glm::vec2*)(byteData + offset + i * totalVertexOffset) = mesh->UVs[i];
            offset += FieldInfo::GetFieldSize(b);
        }

        VBO.ReplaceData(byteData, mesh->VertexCount);
        TriCount = mesh->FaceCount;

        glVertexArrayElementBuffer(ID, p_EBO.Get());

        delete[] byteData;
    }

    void IndexedVAO::Draw(uint32_t instanceCount) const
    {
        if (!instanceCount) return;
        Bind();
        if(instanceCount - 1)
            glDrawElementsInstanced(GL_TRIANGLES, TriCount * 3, GL_UNSIGNED_INT, nullptr, instanceCount);
        else
            glDrawElements(GL_TRIANGLES, TriCount * 3, GL_UNSIGNED_INT, nullptr);
    }

    IndexedVAO::IndexedVAO(Window* window, FieldInfo fields, uint32_t vCount, uint32_t triCount, void* vertices, void* tris) :  VAO(window, fields, vCount, vertices),
                                                                                            p_EBO(window, triCount, 1, (glm::u32vec3*) tris)
    {
        TriCount = triCount;
        glVertexArrayElementBuffer(ID, p_EBO.Get());
    }
}