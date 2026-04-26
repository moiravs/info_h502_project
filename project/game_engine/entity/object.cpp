#include "object.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "renderableEntityMaker.h"
#include "../manager/octreeManager.h"
#include "../renderer/objectRenderer.h"
#include "../renderer/renderer.h"

std::shared_ptr<Object> Object::make(const char *path, const std::string &shader)
{
    return RenderableEntityMaker::makeRenderable<Object, ObjectRenderer>(shader, path);
}

std::shared_ptr<Object> Object::make(float size, float height, const std::string &shader)
{
    const auto renderer = std::make_shared<ObjectRenderer>(shader);
    const auto ret = std::make_shared<Object>(size, height, renderer);
    renderer->registerEntity(ret);
    OctreeManager::get()->put(ret, ret->getPosition());
    return ret;
}

Object::Object(const char *path, const std::shared_ptr<Renderer>& renderer) : RenderableEntity(renderer)
{
    height = computeHeight();

    m_mesh = std::make_shared<Mesh>();

    if (!m_mesh->LoadMesh(path))
    {
        std::cerr << "Assimp failed to load: " << path << std::endl;
    }
    // computeHeight can now iterate through m_mesh.m_Entries
}

Object::Object(float size, float height, const std::shared_ptr<Renderer>& renderer) : RenderableEntity(renderer)
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
    this->RenderableEntity::setPosition(position);
}

void Object::setScale(const glm::vec3 &scale)
{
    this->_scale = scale;
    this->dirty();
}

glm::mat4 Object::getModel() const
{
    return glm::scale(RenderableEntity::getModel(), this->_scale);
}

glm::vec3 Object::getColor() const
{
    return this->_color;
}

void Object::setColor(const glm::vec3 &color)
{
    this->_color = color;
    this->dirty();
}

std::shared_ptr<Mesh> Object::getMesh() const
{
    return this->m_mesh;
}
