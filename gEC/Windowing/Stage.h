//
// Created by scion on 12/12/2022.
//

#pragma once
#include <cstdint>

namespace gE::Windowing
{
    enum class Stage : uint8_t
    {
        Update      = 0b00000001,
        PreZ        = 0b00000010,
        Render      = 0b00000100,
        Shadow      = 0b00001000,
        PostProcess = 0b00010000,
        Cubemap     = 0b00100000,
        CubemapPreZ = 0b01000000,
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