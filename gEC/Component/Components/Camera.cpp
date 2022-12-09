//
// Created by scion on 12/2/2022.
//

#include "Camera.h"
#include "../../Windowing/Window.h"
#include "Transform.h"
#include "PerspectiveCamera.h"

namespace gE::Component
{
    void Camera::Use()
    {
        GetWindow()->CameraManager->SetCamera(this);
    }

    void CameraManager::OnUpdate(double delta)
    {
        ComponentManager::OnUpdate(delta);

        float fov = 0;
        /*if(typeid(p_CurrentCamera) == typeid(PerspectiveCamera))
            fov == ((PerspectiveCamera*) p_CurrentCamera).fov*/

        Transform* transform = p_CurrentCamera->GetOwner()->GetComponent<Transform>();

        CameraData data(glm::inverse(transform->Model), *p_CurrentCamera->GetProjection(),
                        transform->Location, glm::vec4(p_CurrentCamera->GetClipPlanes(), 0, p_Window->GetAspectRatio()));
        p_CameraBuffer.ReplaceData(&data);
    }
}