//
// Created by scion on 12/2/2022.
//

#include "Camera.h"
#include "../../../Windowing/Window.h"

namespace gE::Component
{
    void Camera::Use()
    {
        GetWindow()->CameraManager->SetCamera(this);
    }

    glm::mat4 Camera::GetView() const
    {
        return glm::inverse(GetOwner()->GetComponent<Transform>()->Model);
    }

    void CameraManager::OnUpdate(double delta)
    {
        ComponentManager::OnUpdate(delta);

        Transform* transform = p_CurrentCamera->GetOwner()->GetComponent<Transform>();

        CameraData data(glm::inverse(transform->Model), *p_CurrentCamera->GetProjection(),
                        transform->Location, glm::vec4(p_CurrentCamera->GetWindow()->GetSize(), p_CurrentCamera->GetClipPlanes()));

        p_ViewFrustum = Math::Frustum(data.Projection * data.View);
        p_CameraBuffer.ReplaceData(&data);
    }

    glm::mat4 CameraManager::GetView() const {
        return glm::inverse(p_CurrentCamera->GetOwner()->GetComponent<Transform>()->Model);
    }
}