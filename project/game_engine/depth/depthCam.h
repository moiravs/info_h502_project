
#ifndef INFOH502_CPP_DEPTHCAM_H
#define INFOH502_CPP_DEPTHCAM_H
#include "glm/glm.hpp"

class DepthCam {
public:
    virtual ~DepthCam() = default;
    virtual glm::mat4 getPV() const=0;
};



#endif //INFOH502_CPP_DEPTHCAM_H
