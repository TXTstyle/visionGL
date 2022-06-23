#include "Texture.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <cstdint>
#include "stb/stb_image.h"

using namespace Vision;

Texture::Texture(const std::string& path) 
: width(0), height(0), BPP(0), localBuffer(nullptr) {
    stbi_set_flip_vertically_on_load(1);
    

    localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, STBI_rgb_alpha);
    
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGenTextures(1,&rendererID);
    glBindTexture(GL_TEXTURE_2D, rendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if(localBuffer)
        stbi_image_free(localBuffer);

}

Texture::~Texture() {
    glDeleteTextures(1, &rendererID);
}

void Texture::Bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, rendererID);
}
