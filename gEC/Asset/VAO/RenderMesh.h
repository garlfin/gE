//
// Created by scion on 12/12/2022.
//

#pragma once

#include "Mesh.h"
#include "VAO.h"
#include "IndexedVAO.h"
#include <memory>

namespace gE::Asset
{
    struct RenderMesh final : public Asset
        {
        private:
            RenderMesh(Mesh* mesh, Window* window);
        public:
            RenderMesh(Window* window, gE::Mesh* mesh);
            RenderMesh(Window* window, gE::Mesh* mesh, VAO** vaos);

            const gE::Mesh *const Mesh;
            const gE::Asset::VAO  **const Renderers;

            ~RenderMesh() { delete[] Renderers; }
        };
}