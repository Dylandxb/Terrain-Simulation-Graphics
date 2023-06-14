#pragma once
#include "stdafx.h"


class MemTexture
{
    int width = 0;
    int height = 0;
    unsigned char* image = nullptr;
public:
    glm::vec4 Sample(const glm::vec2& uv)
    {
        glm::vec2 uvMod = uv;
        // clamp to 0, 1
        uvMod.x = fmin(1.0f, fmax(uvMod.x, 0.0f));
        uvMod.y = fmin(1.0f, fmax(uvMod.y, 0.0f));
        int i, j;
        i = (int)(width * uvMod.x);
        j = (int)(height * uvMod.y);
        unsigned char r, g, b, a;
        r = image[(j * width + i) * 4 + 0];                                                             //Texture colours
        g = image[(j * width + i) * 4 + 1];
        b = image[(j * width + i) * 4 + 2];
        a = image[(j * width + i) * 4 + 3];
        return glm::vec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
    }
    MemTexture(const std::string& fileName)
    {
        image = SOIL_load_image(fileName.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);
        if (image == NULL)
        {
            std::cerr << "failed to load texture image from " << fileName << std::endl;
            return;
        }
    }


 

    ~MemTexture() { SOIL_free_image_data(image); }
};