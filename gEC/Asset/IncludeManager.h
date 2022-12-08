//
// Created by scion on 11/23/2022.
//

#pragma once

#include <cstdint>
#include "AssetManager.h"

namespace gE::Asset
{
    class IncludeManager : public AssetManager<std::pair<const char*, const char*>>
    {
    public:
        // Pair: path, value
        const char* LoadIncludesRecurse(const char* path);
        uint32_t FindIncludeFileLength(const char* path) const;
        const char* FindInclude(const char* path) const;

        void Destruct(std::pair<const char*, const char*>* pair) override;
    };
}
