//
// Created by scion on 11/10/2022.
//

#include "Transform.h"
#include <glm/gtx/transform.hpp>

namespace gE::Component
{
    void Transform::OnLoad()
    {
        Model = glm::mat4(1.0f);
    }

    void Transform::OnRender(double delta)
    {
        OnUpdate(delta);
    }

    void Transform::OnUpdate(double delta)
    {
        Model = glm::translate(glm::mat4(1.0), Location);
        Model *= glm::rotate(glm::radians(Rotation.z), glm::vec3(0, 0, 1));
        Model *= glm::rotate(glm::radians(Rotation.y), glm::vec3(0, 1, 0));
        Model *= glm::rotate(glm::radians(Rotation.x), glm::vec3(1, 0, 0));
        Model *= glm::scale(Scale);
    }
}