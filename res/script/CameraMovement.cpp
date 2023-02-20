//
// Created by scion on 12/5/2022.
//

#include <iostream>
#include "CameraMovement.h"
#include "../../gEC/Component/Entity.h"
#include "../../gEC/Windowing/Window.h"
#include "GLFW/glfw3.h"
#include "glm/trigonometric.hpp"

#define CHECK_KEY(k) glfwGetKey(GetWindow()->GetWindow(), (k))

namespace gE::Component
{
    void CameraMovement::OnLoad()
    {
        transform = GetOwner()->GetComponent<Transform>();
    }

    glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float f)
    {
        auto clamped = std::clamp(f, 0.f, 1.f);
        return (1 - clamped) * a + clamped * b;
    }

    void CameraMovement::OnUpdate(double delta)
    {
        glm::dvec2 mousePos;
        glfwGetCursorPos(GetWindow()->GetWindow(), &mousePos.x, &mousePos.y);
        glm::dvec2 mouseDelta = mousePos - prevMousePos;
        transform->Rotation -= glm::vec3(mouseDelta.y, mouseDelta.x, 0) * glm::vec3(0.1);
        transform->Rotation.x = std::clamp(transform->Rotation.x, -89.0f, 89.0f);
        prevMousePos = mousePos;

        glm::vec3 front;
        front.x =  cos(glm::radians(transform->Rotation.x)) * sin(glm::radians(transform->Rotation.y));
        front.y = -sin(glm::radians(transform->Rotation.x));
        front.z =  cos(glm::radians(transform->Rotation.x)) * cos(glm::radians(transform->Rotation.y));
        front = glm::normalize(-front);

        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));

        glm::vec3 direction(0);
        direction += glm::vec3(CHECK_KEY(GLFW_KEY_W) - CHECK_KEY(GLFW_KEY_S)) * front;
        direction += glm::vec3(CHECK_KEY(GLFW_KEY_D) - CHECK_KEY(GLFW_KEY_A)) * right;
        direction.y += CHECK_KEY(GLFW_KEY_E) - CHECK_KEY(GLFW_KEY_Q);

        velocity = lerp(velocity, direction / (glm::length(direction) ?: 1) * 3.f, delta * 10);
        transform->Location += velocity * glm::vec3(delta) ;
    }
}