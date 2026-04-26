
#ifndef INFOH502_CPP_OCTREEMANAGER_H
#define INFOH502_CPP_OCTREEMANAGER_H
#include <memory>

class Octree;

class OctreeManager {
    OctreeManager()=default;
    static std::shared_ptr<Octree> _instance;
public:
    static std::shared_ptr<Octree> get();
};



#endif //INFOH502_CPP_OCTREEMANAGER_H
