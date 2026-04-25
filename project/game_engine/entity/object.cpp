#include "object.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "../manager/octreeManager.h"
#include "../renderer/objectRenderer.h"

std::shared_ptr<Object> Object::make(const char *path, const std::shared_ptr<Renderer>& renderer)
{
    const auto ret = std::make_shared<Object>(path);
    ret->registerRenderer(renderer);
    OctreeManager::get()->put(ret, ret->getPosition());
    return ret;
}

std::shared_ptr<Object> Object::make(float size, float height, const std::shared_ptr<Renderer>& renderer)
{
    const auto ret = std::make_shared<Object>(size, height);
    ret->registerRenderer(renderer);
    OctreeManager::get()->put(ret, ret->getPosition());
    return ret;
}

Object::Object(const char *path) : Entity()
{

    height = computeHeight();

    m_mesh = std::make_shared<Mesh>();

    if (!m_mesh->LoadMesh(path))
    {
        std::cerr << "Assimp failed to load: " << path << std::endl;
    }
    // computeHeight can now iterate through m_mesh.m_Entries
}

Object::Object(float size, float height) : Entity()
{
    m_mesh = std::make_shared<Mesh>();

    std::vector<Vertex> planeVertices;
    std::vector<unsigned int> planeIndices;

    glm::vec3 p1(-size, height, -size);
    glm::vec3 p2(size, height, -size);
    glm::vec3 p3(size, height, size);
    glm::vec3 p4(-size, height, size);

    glm::vec2 t1(0.0f, size);
    glm::vec2 t2(size, size);
    glm::vec2 t3(size, 0.0f);
    glm::vec2 t4(0.0f, 0.0f);

    glm::vec3 n(0.0f, size, 0.0f);

    planeVertices.push_back({p1, t1, n});
    planeVertices.push_back({p2, t2, n});
    planeVertices.push_back({p3, t3, n});
    planeVertices.push_back({p4, t4, n});

    planeIndices = {
        0, 1, 2,
        0, 2, 3};

    Mesh::MeshEntry entry;
    entry.Init(planeVertices, planeIndices);
    entry.MaterialIndex = 0;

    m_mesh->m_Entries.push_back(entry);

    this->height = computeHeight();
}

float Object::computeHeight() const
{
    if (vertices.empty())
        return 0.0f;

    float minY = vertices[0].Position.y;
    float maxY = vertices[0].Position.y;

    for (const auto &vertex : vertices)
    {
        if (vertex.Position.y < minY)
            minY = vertex.Position.y;
        if (vertex.Position.y > maxY)
            maxY = vertex.Position.y;
    }

    return maxY - minY;
}

float Object::getHeight() const
{
    return height;
}

void Object::setPosition(const glm::vec3 &position)
{
    this->Entity::setPosition(position);
    this->model = glm::translate(glm::mat4(1.0f), position);
    if (const auto obj = std::dynamic_pointer_cast<Object>(this->shared_from_this()))
    {
        this->octreeNode->moveObject(obj, this->getPosition());
    }
    this->model = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(this->_scale));
}

void Object::setScale(int scale)
{
    this->_scale = scale;
    this->model = glm::translate(glm::mat4(1.0f), this->getPosition());
    this->model = glm::scale(this->model, glm::vec3(scale));
}

const glm::mat4 &Object::getModel() const
{
    return this->model;
}

void Object::registerRenderer(const std::shared_ptr<Renderer> &renderer)
{
    if (!renderer)
        return;
    if (const auto obj = std::dynamic_pointer_cast<Object>(this->shared_from_this()))
    {
        renderer->registerObject(obj);
    }
}

glm::vec3 Object::getColor() const
{
    return this->_color;
}

void Object::setColor(const glm::vec3 &color)
{
    this->_color = color;
}

void Object::setOctreeNode(const std::shared_ptr<Octree>& node)
{
    this->octreeNode = node;
}
