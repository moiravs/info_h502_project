#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 Texture;
	glm::vec3 Normal;
};

class Object
{
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<Vertex> vertices;

	size_t numVertices;

	GLuint VBO, VAO;
	bool transparent;

	glm::mat4 model = glm::mat4(1.0);

	Object(const char *path, bool transparent = false)
	{
		this->transparent = transparent;
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
	}

	Object(float size, float height, bool transparent = false)
	{
		this->transparent = transparent;
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

		numVertices = vertices.size();
	}

	std::vector<glm::vec3> getPositions()
	{
		return positions;
	};
	std::vector<glm::vec2> getTextures()
	{
		return textures;
	}
	std::vector<glm::vec3> getNormals()
	{
		return normals;
	};
	std::vector<Vertex> getVertices()
	{
		return vertices;
	};

	float getHeight()
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
};
#endif