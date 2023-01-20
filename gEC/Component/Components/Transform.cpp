//
// Created by scion on 11/10/2022.
//

#include "Transform.h"
#include "../Entity.h"
#include <glm/gtx/transform.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

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
        gE::Component::Transform* t = nullptr;
        if(p_Owner->GetParent()) t = p_Owner->GetParent()->GetComponent<gE::Component::Transform>();
        Model = glm::translate(t ? t->Model : glm::mat4(1), Location);
        Model *= glm::rotate(glm::radians(Rotation.z), glm::vec3(0, 0, 1));
        Model *= glm::rotate(glm::radians(Rotation.y), glm::vec3(0, 1, 0));
        Model *= glm::rotate(glm::radians(Rotation.x), glm::vec3(1, 0, 0));
        Model *= glm::scale(Scale);
    }

    void TransformManager::UpdateMatrices(const Entity** entities, uint32_t count)
    {
        p_Info.ObjectCount = count;
        for(uint32_t i = 0; i < count; i++)
        {
            auto* transform = entities[i]->GetComponent<Transform>();
            p_Info.Model[i] = transform->Model;
            p_Info.NormalMatrix[i] = glm::inverseTranspose(glm::mat3(transform->Model));
        }

        p_Buffer.ReplaceData(&p_Info);
    }
}