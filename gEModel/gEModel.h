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
        glm::vec3 Location;
        glm::vec3 Rotation;
        glm::vec3 Scale;
    };

    gE::Mesh* LoadgEMeshFromIntermediate(const char* const path, uint32_t* count);
}