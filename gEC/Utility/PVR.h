//
// Created by scion on 11/3/2022.
//

#pragma once

#include "../Asset/Texture/Texture.h"

namespace gE
{
    class Window;
}

namespace gE::Utility
{
    [[nodiscard]] gE::Asset::Texture* LoadPVR(Window* window, const char* const);
}
