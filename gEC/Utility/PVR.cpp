//
// Created by scion on 11/3/2022.
//

#include "PVR.h"
#include "../Asset/Texture/Texture2D.h"
#include "../Windowing/Window.h"
#include "../Asset/Texture/TextureCube.h"
#include <cstdio>
#include <filesystem>
#include <iostream>

namespace
{
    struct PVR
    {
        uint32_t Version;
        uint32_t Flags;
        gE::Asset::TextureType PixelFormat;
        uint32_t ColorSpace;
        uint32_t ChannelType;
        uint32_t Height;
        uint32_t Width;
        uint32_t Depth;
        uint32_t SurfaceCount;
        uint32_t FaceCount;
        uint32_t MipCount;
        uint32_t MetaDataSize;
    };
}

namespace gE::Utility
{

    gE::Asset::Texture* LoadPVR(Window* window, const char* const path, TextureReturnType* returnType)
    {
        if (!std::filesystem::exists(path)) throw std::runtime_error("Could not find requested file!");
        FILE* file = fopen(path, "rb");

        PVR header{};
        uint64_t byteCount = 0;
        glm::uvec2 size;

        fread(&header, sizeof(PVR) - 4, 1, file);
        if(header.Version != 0x03525650) return nullptr;
        if(header.ColorSpace) *((uint64_t*) &header.PixelFormat) += 44; // I have no clue why enums aren't implicitly cast to their respective supporting types

        fseek(file, header.MetaDataSize, SEEK_CUR);

        size = glm::uvec2(header.Width, header.Height);

        for(uint8_t i = 0; i < header.MipCount; i++, size /= 2)
            byteCount += FormatToCompressionRatio(header.PixelFormat).CalculateBytes(size) * header.FaceCount;

        byteCount *= header.Depth * header.SurfaceCount;
        auto* imageData = new uint8_t[byteCount];

        fread(imageData, 1, byteCount, file);

        auto pos = ftell(file);

        fseek(file, 0, SEEK_END);
        if(ftell(file) - pos != 0) std::cout << "Remaining: " << ftell(file) - pos << std::endl;
        fclose(file);

        if (header.FaceCount == 6 && header.Width != header.Height) throw std::runtime_error("Texture is a cubemap, but has a different width and height!");

        Asset::Texture* texture = header.FaceCount == 6 ?
                                  (Asset::Texture*) new gE::Asset::TextureCube(window, header.Width,
                                                                               Asset::TextureFilterMode::LINEAR,
                                                                               header.PixelFormat, imageData,
                                                                               0, header.MipCount)
                                                        :
                                  (Asset::Texture*) new gE::Asset::Texture2D(window, header.Width, header.Height,
                                                                              Asset::TextureFilterMode::LINEAR,
                                                                              header.PixelFormat, imageData,
                                                                              header.MipCount, header.MipCount);

        if(returnType) *returnType = header.FaceCount == 6 ? TextureReturnType::TextureCubeMap : TextureReturnType::Texture2D;

        delete[] imageData;
        return texture;
    }
}