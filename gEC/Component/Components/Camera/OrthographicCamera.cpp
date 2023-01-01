//
// Created by scion on 12/16/2022.
//

#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace gE::Component
{
    void OrthographicCamera::UpdateProjection()
    {
        Projection = glm::ortho(-Size / 2, Size / 2, -Size / 2, Size / 2, ClipPlanes.x, ClipPlanes.y);
    }
}