//
// Created by scion on 1/19/2023.
//

#include "InventoryScript.h"
#include "../gEC/Component/Entity.h"
#include "../gEC/Component/Components/Renderer.h"
#include "../gEC/Windowing/Window.h"
#include "../../gEC/Component/Components/Camera/PerspectiveCamera.h"
#include <GLFW/glfw3.h>

namespace gE::Component
{
    void InventoryScript::OnLoad()
    {
        p_Owner->GetComponent<Renderer>()->SetRenderMesh(Weapon->Mesh);
        //auto* sight = p_Owner->GetChild("Sight");
        //sight->GetComponent<Renderer>()->SetRenderMesh(Weapon->Sight->Mesh);
        //sight->GetComponent<gE::Component::Transform>()->Set(Weapon->SightAttachmentPoint);
        //sight->GetComponent<gE::Component::Transform>()->Scale = glm::vec3(1);

        _transform = p_Owner->GetComponent<gE::Component::Transform>();
        _transform->Set(Weapon->IdleOffset);
    }

    void InventoryScript::OnUpdate(double delta)
    {
        _swayValue += delta;
        _lerpValue = std::clamp(_lerpValue + float(delta / Weapon->BaseADSSpeed) * (glfwGetKey(GetWindow()->GetWindow(), GLFW_KEY_F) ? 1.0f : -1.0f), 0.0f, 1.0f);

        p_Owner->GetParent()->GetComponent<gE::Component::PerspectiveCamera>()->SetFOV(std::lerp(80, Weapon->Sight->ADSFov, _lerpValue), PerspectiveCamera::Horizontal);
        gE::Transform lerpPos = gE::Lerp
        (
            Weapon->IdleOffset,
            gE::Transform
            (
                glm::vec3
                (
                    0,
                    -(Weapon->SightAttachmentPoint.Location.y + Weapon->Sight->ADSPoint.Location.y),
                    Weapon->Sight->ADSPoint.Location.z + Weapon->SightAttachmentPoint.Location.z
                ),
                glm::vec3(0), Weapon->SightAttachmentPoint.Scale
            ),
            _lerpValue
        );

        _transform->Location = lerpPos.Location + glm::vec3(0, sin(_swayValue * std::numbers::pi) * 0.02 * Weapon->IdleOffset.Scale.x * (1 - _lerpValue), 0);
        _transform->Rotation = lerpPos.Rotation + glm::vec3(sin(_swayValue * std::numbers::pi + std::numbers::pi * 1.45) * (1 - _lerpValue), 0, 0);
    }
}