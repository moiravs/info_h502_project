
#ifndef INFOH502_CPP_POSTPROCESSINGSHADER_H
#define INFOH502_CPP_POSTPROCESSINGSHADER_H
#include "shader.h"

class PostProcessingShader: public Shader {
    GLuint vao = 0;
    GLuint vbo = 0;
protected:
    virtual void linkTextures() const=0;
public:
    PostProcessingShader(const std::string &vertexPath, const std::string &fragmentPath);
    ~PostProcessingShader() override;
    void render() const;
};



#endif //INFOH502_CPP_POSTPROCESSINGSHADER_H
