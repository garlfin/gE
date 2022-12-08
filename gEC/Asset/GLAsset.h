//
// Created by scion on 11/2/2022.
//

#pragma once

#include <cstdint>

namespace gE { class Window; }
namespace gE::Asset
{
    class GLAsset
    {
    protected:
        Window* const Owner;
        uint32_t ID;
    public:
        explicit GLAsset(Window* window) : Owner(window) {};
        virtual ~GLAsset() { ID = 0; }

        [[nodiscard]] uint32_t Get() const { return ID; };
        [[nodiscard]] Window* const GetWindow() const { return Owner; };
    };

    typedef GLAsset Asset;
}