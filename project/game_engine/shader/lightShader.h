
#ifndef INFOH502_CPP_LIGHTSHADER_H
#define INFOH502_CPP_LIGHTSHADER_H
#include "postProcessingShader.h"


class LightShader: public PostProcessingShader {
protected:
    void linkTextures() const override;
public:
    LightShader();
};



#endif //INFOH502_CPP_LIGHTSHADER_H
