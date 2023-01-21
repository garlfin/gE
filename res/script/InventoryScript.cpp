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

        auto* t = p_Owner->GetComponent<gE::Component::Transform>();
        t->Location = Weapon->IdleOffset.Location;
        t->Rotation = Weapon->IdleOffset.Rotation;
        t->Scale = Weapon->IdleOffset.Scale;
    }

    void InventoryScript::OnUpdate(double delta)
    {
        _swayValue += delta;

        auto* t = p_Owner->GetComponent<gE::Component::Transform>();

        _lerpValue = std::clamp(_lerpValue + float(delta / Weapon->ADSTime) * (glfwGetKey(GetWindow()->GetWindow(), GLFW_KEY_F) ? 1.0f : -1.0f), 0.0f, 1.0f);

        p_Owner->GetParent()->GetComponent<gE::Component::PerspectiveCamera>()->SetFOV(std::lerp(80, 65, _lerpValue), PerspectiveCamera::Horizontal);

        gE::Transform lerpPos = gE::Lerp(Weapon->IdleOffset, Weapon->AimOffset, _lerpValue);
        t->Location = lerpPos.Location + glm::vec3(0, sin(_swayValue * std::numbers::pi) * 0.02 * Weapon->IdleOffset.Scale.y * (1 - _lerpValue), 0);
        t->Rotation = lerpPos.Rotation + glm::vec3(sin(_swayValue * std::numbers::pi + std::numbers::pi * 1.25) * (1 - _lerpValue), 0, 0);
    }
}