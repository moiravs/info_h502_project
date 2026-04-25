
#ifndef INFOH502_CPP_IDMANAGER_H
#define INFOH502_CPP_IDMANAGER_H

#include <stddef.h>

class IDManager {
    static size_t id;
    IDManager()=default;
public:
    static size_t generateID();
};

#endif //INFOH502_CPP_IDMANAGER_H
