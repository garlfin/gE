//
// Created by scion on 11/17/2022.
//

#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace gE
{
    struct FieldInfo
    {
        union {
            bool Values[4];
            struct {
                bool HasUV;
                bool HasNormal;
                bool HasTangent;
                bool HasColor;
            };
        };

        [[nodiscard]] uint32_t CalculateSize() const
        {
            return ((uint8_t) HasNormal + HasTangent + HasColor) * sizeof(glm::vec3) + (uint8_t) HasUV * sizeof(glm::vec2);
        }

        [[nodiscard]] static uint8_t GetFieldSize(uint8_t i)
        {
            if(i - 1)
                return sizeof(glm::vec3);
            return sizeof(glm::vec2);
        }
    };

    struct SubMesh
    {
        uint32_t VertexCount;
        FieldInfo Fields;
        uint32_t FaceCount;

        glm::vec3* Positions = nullptr;
        glm::vec2* UVs = nullptr;
        glm::vec3* Normals = nullptr;
        glm::vec3* Tangents = nullptr;
        glm::vec3* Colors = nullptr;
        glm::u32vec3* Indices = nullptr;

        ~SubMesh() { delete[] Positions; delete[] UVs; delete[] Normals; delete[] Tangents; delete[] Colors; delete[] Indices; }

        [[nodiscard]] void* GetField(uint8_t i) const
        {
            switch (i)
            {
                case 0: return Positions;
                case 1: return UVs;
                case 2: return Normals;
                case 3: return Tangents;
                case 4: return Colors;
                default: return nullptr;
            }
        }
    };

    struct Mesh
    {
        const char* Name = nullptr;
        uint8_t SubMeshCount = 0;
        SubMesh* SubMeshes = nullptr;

        ~Mesh() { delete[] SubMeshes; delete[] Name; }
    };
}
