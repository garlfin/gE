//
// Created by scion on 12/14/2022.
//

#pragma once
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Mesh.h"

namespace gE::Math
{
    struct Frustum
    {
        struct Fields
        {
            glm::vec4 Left, Right, Bottom, Top, Near, Far;
            Fields() : Left(1), Right(1), Bottom(1), Top(1), Near(1), Far(1) {}
        };
        union
        {
            glm::vec4 Planes[6];
            Fields FFields;
        };

        explicit Frustum(const glm::mat4& mat)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                FFields.Left[i]   = mat[i][3] + mat[i][0];
                FFields.Right[i]  = mat[i][3] - mat[i][0];
                FFields.Bottom[i] = mat[i][3] + mat[i][1];
                FFields.Top[i]    = mat[i][3] - mat[i][1];
                FFields.Near[i]   = mat[i][3] + mat[i][2];
                FFields.Far[i]    = mat[i][3] - mat[i][2];
            }
        }

private:
       glm::vec3 NegativeVertex(const AABB& aabb, const glm::vec4& normal) const
       {
            glm::vec3 out, min = aabb.Min(), max = aabb.Max();

            out.x = normal.x > 0 ? max.x : min.x;
            out.y = normal.y > 0 ? max.y : min.y;
            out.z = normal.z > 0 ? max.z : min.z;

            return out;
       }
public:
        [[nodiscard]] bool Collide(const gE::Math::AABB& bound) const
        {
            float a = 1.0;
            for(uint8_t i = 0; i < 6 && a >= 0; i++)
                a = glm::dot(glm::vec4(NegativeVertex(bound, Planes[i]), 1), Planes[i]);

            return a >= 0;
        }
    };
}
