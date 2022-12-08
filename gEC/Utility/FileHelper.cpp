//
// Created by scion on 11/29/2022.
//

#include <cstdio>
#include "FileHelper.h"

const char* ReadFile(const char* path, uint32_t* length, bool binary)
{
    FILE* file = fopen(path, binary ? "rb": "r");
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    char* src = new char[*length + 1] {};
    fseek(file, 0, SEEK_SET);
    fread(src, 1, *length, file);
    fclose(file);
    return src;
}
