//
// Created by scion on 11/7/2022.
//

#include "gEModel.h"
#include "gETF.h"
#include "Mesh.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <cfloat>

#define PP_FLAGS (aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_FindInvalidData)

using namespace gE;

Dependency* gETF::GetDependency(uint32_t i)
{
    return &Dependencies[i];
}

Material* gETF::GetMaterial(uint32_t i)
{
    return &Materials[i];
}

Mesh* gETF::GetMesh(uint32_t i)
{
    return &Meshes[i];
}

Mesh* gE::LoadgEMeshFromIntermediate(const char* const path, uint32_t* count)
{
    uint32_t countI;
    if(!count) count = &countI;

    Assimp::Importer importer{};
    const aiScene* scene = importer.ReadFile(path, PP_FLAGS | aiProcess_GenBoundingBoxes);

    std::vector<std::pair<std::vector<aiMesh*>, aiString*>> aiData{};

    for(uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        bool match = false;
        for (auto& pair: aiData)
            if (!strcmp(pair.second->data, scene->mMeshes[i]->mName.data))
            {
                match = true;
                break;
            }

        if (!match)
            aiData.emplace_back(std::vector<aiMesh*>(), &scene->mMeshes[i]->mName);

        aiData.back().first.push_back(scene->mMeshes[i]);
    }

    Mesh* meshes = new Mesh[*count = aiData.size()];

    for(uint32_t i = 0; i < *count; i++)
    {
        Mesh* mesh = &meshes[i];

        mesh->SubMeshCount = aiData[i].first.size();
        mesh->SubMeshes = new SubMesh[aiData[i].first.size()];
        mesh->Name = (const char*) memcpy(new char[aiData[i].second->length + 1] {}, aiData[i].second->data, aiData[i].second->length);

        auto min = glm::vec3(FLT_MAX), max = glm::vec3(-FLT_MAX);

        for(uint32_t s = 0; s < aiData[i].first.size(); s++)
        {
            SubMesh* sMesh = &mesh->SubMeshes[s];
            aiMesh* aMesh = aiData[i].first[s];

            min = glm::min(min, *(glm::vec3*) &aMesh->mAABB.mMin);
            max = glm::max(max, *(glm::vec3*) &aMesh->mAABB.mMax);

            sMesh->VertexCount = aMesh->mNumVertices;
            sMesh->FaceCount = aMesh->mNumFaces;

            sMesh->Fields.HasColor = aMesh->GetNumColorChannels();
            sMesh->Fields.HasUV = aMesh->GetNumUVChannels();
            sMesh->Fields.HasNormal = aMesh->HasNormals();
            sMesh->Fields.HasTangent = aMesh->HasTangentsAndBitangents();

            sMesh->Positions = (glm::vec3*) memcpy(new glm::vec3[sMesh->VertexCount], aMesh->mVertices, sMesh->VertexCount * sizeof(glm::vec3));
            if(sMesh->Fields.HasNormal)
                sMesh->Normals = (glm::vec3*) memcpy(new glm::vec3[sMesh->VertexCount], aMesh->mNormals, sMesh->VertexCount * sizeof(glm::vec3));
            if(sMesh->Fields.HasTangent)
                sMesh->Tangents = (glm::vec3*) memcpy(new glm::vec3[sMesh->VertexCount], aMesh->mTangents, sMesh->VertexCount * sizeof(glm::vec3));
            if(sMesh->Fields.HasUV)
            {
                sMesh->UVs = new glm::vec2[sMesh->VertexCount];
                for (uint32_t u = 0; u < sMesh->VertexCount; u++)
                    sMesh->UVs[u] = *(glm::vec2*) &aMesh->mTextureCoords[0][u];
            }
            if(sMesh->Fields.HasColor)
                sMesh->Colors = (glm::vec3*) memcpy(new glm::vec3[sMesh->VertexCount], aMesh->mColors[0], sMesh->VertexCount * sizeof(glm::vec3));

            sMesh->Indices = new glm::u32vec3[sMesh->FaceCount];
            for(uint32_t f = 0; f < sMesh->FaceCount; f++)
            {
                unsigned* indices = aMesh->mFaces[f].mIndices;
                sMesh->Indices[f] = glm::u32vec3(indices[0], indices[1], indices[2]);
            }
        }

        glm::vec3 center = (min + max) / 2.0f;
        meshes->Bounds = Math::AABB(center, max - center);
    }

    return meshes;
}

