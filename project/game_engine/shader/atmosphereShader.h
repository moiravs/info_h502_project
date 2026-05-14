
#ifndef INFOH502_CPP_AtmosphereShader_H
#define INFOH502_CPP_AtmosphereShader_H
#include "postProcessingShader.h"

class AtmosphereShader : public PostProcessingShader
{
protected:
    void linkTextures() const override;

public:
    AtmosphereShader();
};

#endif // INFOH502_CPP_AtmosphereShader_H
