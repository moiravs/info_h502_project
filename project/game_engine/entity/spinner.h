
#ifndef INFOH502_CPP_SPINNER_H
#define INFOH502_CPP_SPINNER_H
#include "object.h"

class Spinner : public Object {
    float _dyaw;
    float _dpitch;
    float _droll;
public:
    explicit Spinner(const std::shared_ptr<Mesh> &mesh, float dyaw, float dpitch, float droll,
        const std::shared_ptr<Renderer> &renderer);

    void update(float delta) override;
};



#endif //INFOH502_CPP_SPINNER_H
