//
// Created by scion on 12/12/2022.
//

#include "RenderMesh.h"

gE::Asset::RenderMesh::RenderMesh(gE::Window* window, gE::Mesh* mesh) : RenderMesh(mesh, window)
{
    for(uint8_t i = 0; i < mesh->SubMeshCount; i++)
        Renderers[i] = new IndexedVAO(window, &mesh->SubMeshes[i]);
}

gE::Asset::RenderMesh::RenderMesh(Window* window, gE::Mesh* mesh, gE::Asset::VAO** vaos) : RenderMesh(mesh, window) { memcpy(Renderers, vaos, sizeof(VAO*) * mesh->SubMeshCount); }

gE::Asset::RenderMesh::RenderMesh(gE::Mesh* mesh, Window* window) : Asset(window), Mesh(mesh), Renderers(new const gE::Asset::VAO*[mesh->SubMeshCount])
{}
