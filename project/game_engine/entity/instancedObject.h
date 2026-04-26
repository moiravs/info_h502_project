
#ifndef INFOH502_CPP_INSTANCEDOBJECT_H
#define INFOH502_CPP_INSTANCEDOBJECT_H

#include "object.h"
#include "../renderer/instancedRenderer.h"

class InstancedObject : public Object {
    std::vector<glm::mat4> _models {};
public:
    static std::shared_ptr<InstancedObject> make(const std::shared_ptr<Mesh> &mesh, const std::string &shader, const std::vector<glm::mat4>& models);
    InstancedObject(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer>& renderer);
    void setModels(const std::vector<glm::mat4>& models);
    std::shared_ptr<InstancedRenderer> getInstancedRenderer() const;
};



#endif //INFOH502_CPP_INSTANCEDOBJECT_H
