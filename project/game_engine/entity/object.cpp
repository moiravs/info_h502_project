#include "object.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "../renderer/objectRenderer.h"

std::shared_ptr<Object> Object::make(const char *path, std::shared_ptr<Renderer> renderer)
{
    const auto ret = std::make_shared<Object>(path);
    ret->registerRenderer(renderer);
    return ret;
}

std::shared_ptr<Object> Object::make(float size, float height, std::shared_ptr<Renderer> renderer)
{
    const auto ret = std::make_shared<Object>(size, height);
    ret->registerRenderer(renderer);
    return ret;
}

Object::Object(const char *path) : Entity()
{
    std::ifstream infile(path);
    if (infile.is_open())
    {
        std::string line;
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            std::string indice;
            iss >> indice;
            if (indice == "v")
            {
                float x, y, z;
                iss >> x >> y >> z;
                positions.emplace_back(x, y, z);
            }
            else if (indice == "vn")
            {
                float x, y, z;
                iss >> x >> y >> z;
                normals.emplace_back(x, y, z);
            }
            else if (indice == "vt")
            {
                float u, v;
                iss >> u >> v;
                textures.emplace_back(u, v);
            }
            else if (indice == "f")
            {
                std::string f1, f2, f3;
                iss >> f1 >> f2 >> f3;

                std::string p, t, n;

                // for vertex 1
                Vertex v1{};

                p = f1.substr(0, f1.find('/'));
                f1.erase(0, f1.find('/') + 1);

                t = f1.substr(0, f1.find('/'));
                f1.erase(0, f1.find('/') + 1);

                n = f1.substr(0, f1.find('/'));

                v1.Position = positions.at(std::stoull(p) - 1);
                v1.Normal = normals.at(std::stoull(n) - 1);
                v1.Texture = textures.at(std::stoull(t) - 1);
                vertices.push_back(v1);

                // for vertex 2
                Vertex v2{};

                p = f2.substr(0, f2.find('/'));
                f2.erase(0, f2.find('/') + 1);

                t = f2.substr(0, f2.find('/'));
                f2.erase(0, f2.find('/') + 1);

                n = f2.substr(0, f2.find('/'));

                v2.Position = positions.at(std::stoull(p) - 1);
                v2.Normal = normals.at(std::stoull(n) - 1);
                v2.Texture = textures.at(std::stoull(t) - 1);
                vertices.push_back(v2);

                // for vertex 3
                Vertex v3{};

                p = f3.substr(0, f3.find('/'));
                f3.erase(0, f3.find('/') + 1);

                t = f3.substr(0, f3.find('/'));
                f3.erase(0, f3.find('/') + 1);

                n = f3.substr(0, f3.find('/'));

                v3.Position = positions.at(std::stoull(p) - 1);
                v3.Normal = normals.at(std::stoull(n) - 1);
                v3.Texture = textures.at(std::stoull(t) - 1);
                vertices.push_back(v3);
            }
        }
        infile.close();
    }
    else
    {
        std::cout << "Error opening file: " << path << std::endl;
    }

    height = computeHeight();
}

Object::Object(float size, float height) : Entity()
{
    // 4 corners of the plane
    glm::vec3 p1(-size, height, -size);
    glm::vec3 p2(size, height, -size);
    glm::vec3 p3(size, height, size);
    glm::vec3 p4(-size, height, size);

    // Texture coordinates
    glm::vec2 t1(0.0f, size);
    glm::vec2 t2(size, size);
    glm::vec2 t3(size, 0.0f);
    glm::vec2 t4(0.0f, 0.0f);

    // Normal (pointing straight up)
    glm::vec3 n(0.0f, size, 0.0f);

    // Triangle 1
    vertices.push_back({p1, t1, n});
    vertices.push_back({p2, t2, n});
    vertices.push_back({p3, t3, n});

    // Triangle 2
    vertices.push_back({p1, t1, n});
    vertices.push_back({p3, t3, n});
    vertices.push_back({p4, t4, n});

    height = computeHeight();
}

std::vector<glm::vec3> Object::getPositions()
{
    return positions;
}

std::vector<glm::vec2> Object::getTextures()
{
    return textures;
}

std::vector<glm::vec3> Object::getNormals()
{
    return normals;
}

std::vector<Vertex> Object::getVertices()
{
    return vertices;
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

size_t Object::getNumVertices() const
{
    return this->vertices.size();
}

void Object::setPosition(const glm::vec3 &position)
{
    this->Entity::setPosition(position);
    this->model = glm::translate(glm::mat4(1.0f), position);
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