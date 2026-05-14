
#include "atmosphereShader.h"

AtmosphereShader::AtmosphereShader()
    : PostProcessingShader(PATH_TO_SRC "/../assets/shaders/atmosphere.vert",
                           PATH_TO_SRC "/../assets/shaders/atmosphere.frag") {}

void AtmosphereShader::linkTextures() const
{
    this->use();
    glUniform1i(glGetUniformLocation(this->getID(), "color"), COLOR_TEX_IDX);
    glUniform1i(glGetUniformLocation(this->getID(), "normal"), NORMAL_TEX_IDX);
    glUniform1i(glGetUniformLocation(this->getID(), "depth"), DEPTH_TEX_IDX);
    glUniform1i(glGetUniformLocation(this->getID(), "material"), MATERIAL_TEX_IDX);
    glUniform1i(glGetUniformLocation(this->getID(), "shadow"), SHADOW_TEX_IDX);
}
