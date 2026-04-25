
#ifndef INFOH502_CPP_OCTREE_H
#define INFOH502_CPP_OCTREE_H
#include <memory>
#include "../entity/object.h"

class Object;

class Octree : public std::enable_shared_from_this<Octree> {
    std::array<std::shared_ptr<Octree>, 8> children;
    std::shared_ptr<Octree> parent = nullptr;
    std::vector<std::shared_ptr<Object>> data {};
    glm::vec3 center;
    glm::vec3 halfDim;
    glm::vec3 minBound;
    glm::vec3 maxBound;

    short getSubOctreeContaining(const glm::vec3& point) const;
    void put(const std::vector<std::shared_ptr<Object>>& objects, const glm::vec3& position);
    void moveObjectUp(const std::shared_ptr<Object>& object, const glm::vec3& position);
    std::pair<bool, std::vector<std::shared_ptr<Object>>> childrenAllLeafAndSmashable() const;

public:
    Octree(const glm::vec3& center, const glm::vec3& halfDim, std::shared_ptr<Octree> parent = nullptr);
    bool isInOctree(const glm::vec3& point) const;
    void put(const std::shared_ptr<Object>& object, const glm::vec3& position);
    void moveObject(const std::shared_ptr<Object>& object, const glm::vec3& newPosition);

    std::array<std::shared_ptr<Octree>, 8> getChildren();
    std::vector<std::shared_ptr<Object>> getData();
    glm::vec3 getMinBound() const;
    glm::vec3 getMaxBound() const;
    bool isLeaf() const;
};



#endif //INFOH502_CPP_OCTREE_H
