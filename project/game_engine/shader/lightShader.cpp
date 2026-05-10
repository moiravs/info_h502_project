
#include "lightShader.h"

LightShader::LightShader()
: PostProcessingShader(PATH_TO_SRC "/../assets/shaders/light.vert",
    PATH_TO_SRC "/../assets/shaders/light.frag") {  }

void LightShader::linkTextures() const
{
    this->use();
    glUniform1i(glGetUniformLocation(this->getID(), "color"), 0);
    glUniform1i(glGetUniformLocation(this->getID(), "normal"), 1);
    glUniform1i(glGetUniformLocation(this->getID(), "depth"), 2);
    glUniform1i(glGetUniformLocation(this->getID(), "shadow"), 3);
}
