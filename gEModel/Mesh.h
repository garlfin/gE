//
// Created by scion on 11/17/2022.
//

#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace gE
{
    namespace Math
    {
        struct AABB
        {
            glm::vec3 Center;
            glm::vec3 Extent;

            AABB() : Center(0), Extent(1) {}
            AABB(glm::vec3 const& center, glm::vec3 const& extent) : Center(center), Extent(glm::max(glm::abs(extent), glm::vec3(0.01))) {}
            AABB(AABB const& a, glm::mat4 const& b) : Center(a.Center), Extent(a.Extent) { Transform(b); }

            void Transform(glm::mat4 const& b)
            {
                Center = b * glm::vec4(Center, 1);
                Extent = b * glm::vec4(Extent, 0);
            };

            [[nodiscard]] glm::vec3 Max() const { return Center + Extent; }
            [[nodiscard]] glm::vec3 Min() const { return Center - Extent; }

        };
    }

    struct FieldInfo
    {
        union {
            bool Values[5];
            struct {
                bool HasPosition;
                bool HasUV;
                bool HasNormal;
                bool HasTangent;
                bool HasColor;
            };
        };

        FieldInfo(bool hasUv, bool hasNormal, bool hasTangent, bool hasColor) : HasPosition(true), HasUV(hasUv),
                                                                                HasNormal(hasNormal),
                                                                                HasTangent(hasTangent),
                                                                                HasColor(hasColor) {}

        [[nodiscard]] uint32_t CalculateSize() const
        {
            return ((uint8_t) HasPosition + HasNormal + HasTangent + HasColor) * sizeof(glm::vec3) + (uint8_t) HasUV * sizeof(glm::vec2);
        }

        [[nodiscard]] static uint8_t GetFieldSize(uint8_t i)
        {
            if(i == 1)
                return sizeof(glm::vec2);
            return sizeof(glm::vec3);
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

        SubMesh() : Fields(false, false, false, false), VertexCount(), FaceCount() { }
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
        Math::AABB Bounds;

        ~Mesh() { delete[] SubMeshes; delete[] Name; }
    };
}
