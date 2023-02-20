//
// Created by scion on 11/23/2022.
//

#include <cstring>
#include "IncludeManager.h"
#include <stdexcept>
#include <filesystem>
#include <iostream>
#include "../Utility/FileHelper.h"

#define INC_LEN 11
#define INC "\n#include "

uint32_t strlenc(const char* str, char cr)
{
    uint32_t len = 0;
    for(;;str++)
        if(*str != cr) len++;
        else if(*str == 0 || *str == cr) return len;
}

namespace gE::Asset
{
    uint32_t IncludeManager::FindIncludeFileLength(const char* path) const
    {
        if (const char* include = FindInclude(path))
            return strlen(include);
        return 0;
    }

    const char* IncludeManager::FindInclude(const char* path) const
    {
        for (std::pair<const char*, const char*>* pair: p_Assets)
            if (!strcmp(pair->first, path))
                return pair->second;
        return nullptr;
    }

    const char* IncludeManager::LoadIncludesRecurse(const char* path)
    {
        if (const char* incData = FindInclude(path)) return incData;
        if (!std::filesystem::exists(path)) throw std::runtime_error("Could not find requested file!");

        uint32_t shaderSourceLength;
        const char* const shaderSource = ReadFile(path, &shaderSourceLength, true);

        auto* pair = new std::pair<const char*, const char*>(path, shaderSource);
        p_Assets.push_back(pair);

        // Search
        uint32_t includeCount = 0;
        for (const char* directive = shaderSource; (directive = strstr(directive, INC)); includeCount++) directive++;
        const char** includes = new const char* [includeCount];

        {
            const char* directive = shaderSource;
            for (uint32_t i = 0; (directive = strstr(directive, "\n#include ")); i++, directive++)
            {
                char delimiter = *(directive + 10);

                if (delimiter != '"' && delimiter != '<')
                    throw std::runtime_error("Invalid delimiter following #include!");

                uint16_t pathLen = strlenc(directive + INC_LEN, delimiter == '"' ? '"' : '>');
                char* directivePath = (char*) memcpy(new char[pathLen + 1]{}, directive + INC_LEN, pathLen);

                includes[i] = directivePath;
                LoadIncludesRecurse(directivePath);
            }
        }

        if (!includeCount) return shaderSource;

        // Calculate final length
        uint32_t length = FindIncludeFileLength(path);
        for (uint32_t i = 0; i < includeCount; i++)
            length += FindIncludeFileLength(includes[i]) - INC_LEN - strlen(includes[i]) + 1;

        const char* directive, * previousDirective;
        directive = previousDirective = shaderSource;

        char* assemblyLocation;
        pair->second = assemblyLocation = new char[length + 1]{};

        // Assembly
        for (uint32_t i = 0; (directive = strstr(directive, INC)); i++, previousDirective = directive)
        {
            memcpy(assemblyLocation, previousDirective, directive - previousDirective);
            assemblyLocation += directive - previousDirective;
            directive += INC_LEN + strlen(includes[i]) + 1;

            *assemblyLocation = '\n'; // Just in case [-1] is some sort of undefined behavior
            assemblyLocation++;

            memcpy(assemblyLocation, FindInclude(includes[i]), FindIncludeFileLength(includes[i]));
            assemblyLocation += FindIncludeFileLength(includes[i]);
        }

        memcpy(assemblyLocation, previousDirective, strlen(previousDirective));
        assemblyLocation += strlen(previousDirective);
        if (assemblyLocation > pair->second + length + 1) std::cout << "Wrote to unallocated memory!" << std::endl;

        delete[] shaderSource;
        return pair->second;
    }

    void IncludeManager::Destruct(std::pair<const char *, const char *> *pair)
    {
        delete pair->first;
        delete pair->second;
        delete pair;
    }
}