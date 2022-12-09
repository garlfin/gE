//
// Created by scion on 11/30/2022.
//

#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../Windowing/Window.h"
#include "Transform.h"

namespace gE::Component
{
    void PerspectiveCamera::UpdateProjection()
    {
        Projection = glm::perspectiveFov(glm::radians(FOV), (float) GetWindow()->GetSize().x, (float) GetWindow()->GetSize().y, ClipPlanes.x, ClipPlanes.y);
    }

    void PerspectiveCamera::SetFOV(float fovDegrees, PerspectiveCamera::FOVType type)
    {
        FOV = type == Horizontal ? fovDegrees : glm::degrees(atan(tan(glm::radians(fovDegrees) / 2) / GetWindow()->GetAspectRatio()) * 2);
        UpdateProjection();
    }

    void PerspectiveCamera::OnUpdate(double delta)
    {
        if (auto* transform = p_Owner->GetComponent<Transform>())
            View = glm::inverse(transform->Model);
        else
            View = glm::mat4(1);
    }

    PerspectiveCamera::PerspectiveCamera(Entity* owner, float fov, glm::vec2 planes) : Camera(owner, planes), FOV(fov)
    {
        UpdateProjection();
    }
}
