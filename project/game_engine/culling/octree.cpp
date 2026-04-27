
#include "octree.h"

#include "../../utils/utils.h"

// Inspired by
// https://github.com/OpenGL-Graphics/octree/blob/master/src/octree.cpp

Octree::Octree(const glm::vec3& center, const glm::vec3& halfDim, std::shared_ptr<Octree> parent)
: parent(std::move(parent)), center(center), halfDim(halfDim), minBound(center - halfDim), maxBound(center + halfDim)
{}

bool Octree::isLeaf() const
{
    return this->children.at(0) == nullptr;
}

bool Octree::isInOctree(const glm::vec3& point) const
{
    return isInVolume(point, this->minBound, this->maxBound);
}

short Octree::getSubOctreeContaining(const glm::vec3& point) const
{
    if (!isInOctree(point)) return -1;

    int oct = 0;
    if (point.x >= this->center.x) oct |= 4;
    if (point.y >= this->center.y) oct |= 2;
    if (point.z >= this->center.z) oct |= 1;
    return oct;
}

void Octree::put(const std::shared_ptr<RenderableEntity>& object, const glm::vec3& position)
{
    const std::vector toPut = {object};
    this->put(toPut, position);
}

void Octree::put(const std::vector<std::shared_ptr<RenderableEntity>>& objects, const glm::vec3& position)
{
    const short subOctreeIndexNew = this->getSubOctreeContaining(position);
    if (subOctreeIndexNew < 0)
    {
        ERROR("Point not in octree."); return;
    }

    // if we're not a leaf, just go through to the next node
    if (!this->isLeaf())
    {
        this->children.at(subOctreeIndexNew)->put(objects, position); return;
    }

    // we're a leaf, if there is no data OR it's at the exact same position just put the point here
    if (this->data.size() == 0 || this->data.at(0)->getPosition() == position)
    {
        for (const auto& o: objects)
        {
            this->data.push_back(o);
            o->setOctreeNode(this->shared_from_this());
        }

        return;
    }

    // if there already is data, this note should not be a leaf anymore, and its data is passed to the new children
    for (short i = 0; i < 8; i++) {
        glm::vec3 newOrigin = this->center;
        newOrigin.x += this->halfDim.x * (i & 4 ? .5f : -.5f);
        newOrigin.y += this->halfDim.y * (i & 2 ? .5f : -.5f);
        newOrigin.z += this->halfDim.z * (i & 1 ? .5f : -.5f);
        this->children[i] = std::make_shared<Octree>(newOrigin, this->halfDim * .5f, this->shared_from_this());
    }

    const glm::vec3 currentDataPosition = this->data.at(0)->getPosition();
    const short subOctreeIndexOld = this->getSubOctreeContaining(currentDataPosition);

    this->children.at(subOctreeIndexOld)->put(this->data, currentDataPosition);
    this->children.at(subOctreeIndexNew)->put(objects, position);

    this->data = {};
}

void Octree::moveObject(const std::shared_ptr<RenderableEntity>& object, const glm::vec3& newPosition)
{
    auto it = this->data.begin();
    for (; it != this->data.end(); ++it)
    {
        if (object.get() == it->get()) break;
    }

    if (it == this->data.end())
    {
        FATAL("Object not found!"); return;
    }

    // Move found element to the end, then remove it
    std::iter_swap(it, this->data.end() - 1);
    this->data.pop_back();

    this->moveObjectUp(object, newPosition);
}

void Octree::moveObjectUp(const std::shared_ptr<RenderableEntity>& object, const glm::vec3& position)
{
    auto [shouldSmash, childData] = this->childrenAllLeafAndSmashable();
    if (shouldSmash)
    {
        for (auto& c : this->children) c = nullptr;
        this->data = childData;
        for (const auto &c : this->data)
            c->setOctreeNode(this->shared_from_this());
    }

    if (this->isInOctree(position))
        this->put(object, position);
    else if (this->parent)
        this->parent->moveObjectUp(object, position);
    else
        ERROR("No parent.");
}

std::pair<bool, std::vector<std::shared_ptr<RenderableEntity>>> Octree::childrenAllLeafAndSmashable() const
{
    if (this->isLeaf()) return {false, {}};

    bool foundData = false;
    std::vector<std::shared_ptr<RenderableEntity>> objects;

    for (const auto& c : this->children)
    {
        if (c != nullptr && !c->isLeaf())
            return {false, {}};

        if (c != nullptr && !c->data.empty())
        {
            if (foundData)
                return {false, {}};

            objects = c->data;
            foundData = true;
        }
    }

    return {true, objects};
}

std::array<std::shared_ptr<Octree>, 8> Octree::getChildren()
{
    return this->children;
}

std::vector<std::shared_ptr<RenderableEntity>> Octree::getData()
{
    return this->data;
}

glm::vec3 Octree::getMaxBound() const
{
    return this->maxBound;
}

glm::vec3 Octree::getMinBound() const
{
    return this->minBound;
}

void Octree::print(const int depth) const
{
    // indentation
    std::string indent(depth * 2, ' ');

    // node info
    std::cout << indent
              << "[Node] "
              << (isLeaf() ? "Leaf" : "Internal")
              << " | Objects: " << data.size()
              << " | Min: (" << minBound.x << ", " << minBound.y << ", " << minBound.z << ")"
              << " | Max: (" << maxBound.x << ", " << maxBound.y << ", " << maxBound.z << ")"
              << std::endl;

    // print objects (optional but useful)
    for (const auto& obj : data)
    {
        const auto& pos = obj->getPosition();
        std::cout << indent << "  - Obj @ ("
                  << pos.x << ", " << pos.y << ", " << pos.z << ")"
                  << std::endl;
    }

    // recurse into children
    if (!isLeaf())
    {
        for (int i = 0; i < 8; ++i)
        {
            if (children[i])
            {
                std::cout << indent << " Child[" << i << "]:" << std::endl;
                children[i]->print(depth + 1);
            }
        }
    }
}