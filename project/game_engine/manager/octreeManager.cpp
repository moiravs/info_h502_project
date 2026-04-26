
#include "octreeManager.h"

#include "../culling/octree.h"

std::shared_ptr<Octree> OctreeManager::_instance = nullptr;

std::shared_ptr<Octree> OctreeManager::get()
{
    if (_instance == nullptr)
        _instance = std::make_shared<Octree>(glm::vec3(0, 0, 0),
    glm::vec3(PLAN_SIZE_X / 2, PLAN_SIZE_X / 2, PLAN_SIZE_X / 2));

    return _instance;
}
