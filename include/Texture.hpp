#pragma once

#include <string>

namespace Vision
{
    class Texture
    {
    private:
        unsigned int rendererID;
        std::string filepath;
        int width, height, BPP;
        unsigned char* localBuffer;
    public:
        Texture(const std::string& path);
        ~Texture();

        void Bind(unsigned int slot = 0);

        inline unsigned int GetID() const { return rendererID; };
        inline int GetWidth() const { return width; };
        inline int GetHeight() const { return height; };
    };
} 


