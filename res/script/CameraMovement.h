//
// Created by scion on 12/5/2022.
//

#pragma once

#include "../../gEC/Component/Components/Behavior.h"
#include "../../gEC/Component/Components/Transform.h"
#include "../../gEC/Asset/Shader/Shader.h"
#include "../../gEC/Asset/Texture/Texture.h"

namespace gE::Component
{
    class CameraMovement final : public Behavior
    {
    private:
        Transform* transform = nullptr;
        glm::dvec2 prevMousePos = glm::dvec2(0);
        glm::vec3 velocity = glm::vec3(0);
        Asset::Shader* compositeShader;
        Entity* fpsCam;

        Asset::Texture* _TAABufferTex;


    public:
        explicit CameraMovement(Entity* owner, Entity* fpsCamera) : Behavior(owner), fpsCam(fpsCamera) {}
        void OnLoad() override;
        void OnUpdate(double delta) override;
        void OnRender(double delta) override;

        ~CameraMovement() override { delete compositeShader; delete _TAABufferTex; }
    };
}
