//
// Created by scion on 1/19/2023.
//

#include "InventoryScript.h"
#include "../gEC/Component/Entity.h"
#include "../gEC/Component/Components/Renderer.h"
#include "../gEC/Windowing/Window.h"
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

        if(glfwGetKey(GetWindow()->GetWindow(), GLFW_KEY_F))
            _lerpValue = std::clamp(_lerpValue + float(delta / Weapon->ADSTime), 0.0f, 1.0f);
        else
            _lerpValue = std::clamp(_lerpValue - float(delta / Weapon->ADSTime), 0.0f, 1.0f);

        gE::Transform lerpPos = gE::Lerp(Weapon->IdleOffset, Weapon->AimOffset, _lerpValue);
        t->Location = lerpPos.Location + glm::vec3(0, sin(_swayValue * 3) * 0.01 * Weapon->IdleOffset.Scale.y * (1 - _lerpValue), 0);
        t->Rotation = lerpPos.Rotation;
    }
}