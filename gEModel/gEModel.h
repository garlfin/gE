#pragma once

#include <cstdint>
#include <cstring>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "Mesh.h"

namespace gE
{
    enum class CompressionFormat : uint8_t
    {
        None = 0,
        LZMA = 1
    };

    enum class DependencyType : uint8_t
    {
        Texture = 0,
        ShaderProgram = 1,
        Shader = 2,
        gETF = 3
    };

    struct Transform
    {
        Transform() : Location(0), Rotation(0), Scale(1) {};
        Transform(const glm::vec3& loc, const glm::vec3& rot, const glm::vec3& sca) : Location(loc), Rotation(rot), Scale(sca) {};

        glm::vec3 Location;
        glm::vec3 Rotation;
        glm::vec3 Scale;

        Transform operator *(float b)
        {
            return Transform(Location * b, Rotation, Scale * b);
        };

        Transform operator +(const Transform& b)
        {
            return Transform(Location + b.Location, Rotation + b.Rotation, Scale);
        };

        Transform operator -(const Transform& b)
        {
            return Transform(Location - b.Location, Rotation - b.Rotation, Scale);
        };

        Transform operator -()
        {
            return Transform(Location * -1.0f, Rotation, Scale);
        };

    };

#ifndef GEMODEL_LERP
#define GEMODEL_LERP 1
    inline Transform Lerp(const Transform& a, const Transform& b, float c)
    {
        return Transform(glm::mix(a.Location, b.Location, c), glm::mix(a.Rotation, b.Rotation, c), glm::mix(a.Scale, b.Scale, c));
    }
#endif

    gE::Mesh* LoadgEMeshFromIntermediate(const char* const path, uint32_t* count = nullptr);
}