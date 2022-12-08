//
// Created by scion on 11/17/2022.
//

#pragma once

#include <cstdint>
#include "gEModel.h"
#include "Mesh.h"

namespace gE
{
    struct Dependency
    {
        const char* InternalPath; // Internal file system path
        gE::DependencyType Type;

        bool IsPacked;

        // if (IsPacked)
        gE::CompressionFormat Compression;
        uint64_t CompressedSize;
        uint64_t UncompressedSize;
        uint8_t* Data;

        // else
        const char* Path; // Real file system path

        ~Dependency() { delete[] Data; }
    };

    struct Material
    {
        const char*  Name;
        uint16_t DependencyID;
    };

    struct Instance
    {
        Instance* Parent = nullptr;
        Instance** Children = nullptr;

        uint16_t MeshIndex = 0;
        gE::Transform Transform;
    };

    struct Scene
    {
        uint32_t InstanceCount;
        Instance* Instances;

        ~Scene() { delete[] Instances; }
    };

    struct gETF
    {
        char Magic[4]{'g', 'E', 'T', 'F'};
        uint16_t Version;

        uint16_t DependencyCount = 0;
        uint16_t MaterialCount = 0;
        uint16_t MeshCount = 0;

        Dependency* Dependencies = nullptr;
        Material* Materials = nullptr;
        Mesh* Meshes = nullptr;

        Dependency* GetDependency(uint32_t);
        Material* GetMaterial(uint32_t);
        Mesh* GetMesh(uint32_t);
        Scene MainScene;

        ~gETF() { delete[] Dependencies; delete[] Materials; delete[] Meshes; }
    };
}