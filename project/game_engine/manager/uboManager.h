
#ifndef INFOH502_CPP_UBOHANDLER_H
#define INFOH502_CPP_UBOHANDLER_H
#include "../shader.h"


class UboManager {
    UboManager();

    std::unordered_map<std::string, GLuint> bindings{};
public:
    static UboManager& get();
    void registerShader(const Shader* shader);
    GLuint getBinding(const std::string& name) const;
};



#endif //INFOH502_CPP_UBOHANDLER_H
