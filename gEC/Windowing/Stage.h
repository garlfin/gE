//
// Created by scion on 12/12/2022.
//

#pragma once
#include <cstdint>

namespace gE::Windowing
{
    enum class Stage : uint8_t
    {
        Update      = 1,
        PreZ        = 2,
        Render      = 8,
        Shadow      = 16,
        PostProcess = 32,
        Cubemap = 64,
    };

    inline Stage operator|(Stage a, Stage b)
    {
        return (Stage)((uint8_t)(a) | (std::uint8_t)(b));
    }

    inline uint8_t operator&(Stage a, Stage b)
    {
        return ((uint8_t)(a) & (std::uint8_t)(b));
    }
}