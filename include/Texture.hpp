#pragma once

#include <string>

namespace Vision
{
    struct Texture
    {
        unsigned int renderID;
        uint32_t width, height, BPP;
        uint32_t format, imgFormat;
        uint32_t wrapS, wrapT;
        uint32_t filterMin, filterMax;

        Texture();
        inline unsigned int GetID() const{ return renderID; }

        void Generate(uint32_t width, uint32_t height, unsigned char* data);

        void Bind() const;
    };
} 


