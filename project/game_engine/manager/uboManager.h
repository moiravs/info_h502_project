
#ifndef INFOH502_CPP_UBOHANDLER_H
#define INFOH502_CPP_UBOHANDLER_H
#include "../shader.h"


class UboManager {
    UboManager();

    static UboManager instance;
    std::unordered_map<std::string, GLuint> bindings{};
public:
    static void registerShader(const Shader* shader);
    static GLuint getBinding(const std::string& name);
};



#endif //INFOH502_CPP_UBOHANDLER_H
