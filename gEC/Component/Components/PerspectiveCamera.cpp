//
// Created by scion on 11/30/2022.
//

#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../Windowing/Window.h"
#include "Transform.h"

#define RIGHT glm::vec3(1, 0, 0)
#define UP glm::vec3(0, 1, 0)
#define FRONT glm::vec3(0, 0, 1)

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
        auto* transform = p_Owner->GetComponent<Transform>();
        glm::vec3 rotation = transform ? transform->Rotation : glm::vec3(0.0);

        // The ship stays where it is, and the engines move the universe around it.
        // -Gus Fring
        // Go backwards, if not I rotate around the origin
        View = glm::rotate(glm::radians(-rotation.x), RIGHT);
        View *= glm::rotate(glm::radians(-rotation.y), UP);
        View *= glm::rotate(glm::radians(-rotation.z), FRONT);
        View *= glm::translate(-transform->Location);
    }

    PerspectiveCamera::PerspectiveCamera(Entity* owner, float fov, glm::vec2 planes) : Camera(owner, planes), FOV(fov)
    {
        UpdateProjection();
    }
}
