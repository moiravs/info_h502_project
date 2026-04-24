
#ifndef INFOH502_CPP_UBOPROVIDER_H
#define INFOH502_CPP_UBOPROVIDER_H
#include <string>

#include "glad/glad.h"


class UboProvider {
    long bufferSize;
protected:
    GLuint ubo{};
public:
    explicit UboProvider(const std::string& bindingName, long bufferSize);
    virtual ~UboProvider();
};



#endif //INFOH502_CPP_UBOPROVIDER_H
