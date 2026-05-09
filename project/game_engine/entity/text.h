#ifndef INFOH502_CPP_TEXT_H
#define INFOH502_CPP_TEXT_H

#include <iostream>
#include "../../utils/constants.h"
#include "glm/glm.hpp"
#include <map>
#include <freetype/ftcolor.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text
{

public:
    unsigned int VAO, VBO;

    std::map<GLchar, Character> Characters;

    Text();

    void loadCharactersFromBitmap(FT_Library ft, std::string font_name);
};

#endif