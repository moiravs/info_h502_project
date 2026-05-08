
#include "spinner.h"

Spinner::Spinner(const std::shared_ptr<Mesh>& mesh, const float dyaw, const float dpitch, const float droll,
    const std::shared_ptr<Renderer>& renderer)
    : Object(mesh, renderer), _dyaw(dyaw), _dpitch(dpitch), _droll(droll) {
}

void Spinner::update(const float delta)
{
    this->rotate(this->_dyaw * delta, this->_dpitch * delta, this->_droll * delta);

    Object::update(delta);
}
