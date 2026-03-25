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

	int numVertices;

	GLuint VBO, VAO;
	bool transparent;

	glm::mat4 model = glm::mat4(1.0);

	Object(const char *path, bool transparent = false)
	{
		transparent = transparent;
		std::ifstream infile(path);
		std::string line;
		if (infile.is_open())
		{
			while (std::getline(infile, line))
			{
				std::istringstream iss(line);
				std::string indice;
				iss >> indice;
				if (indice == "v")
				{
					float x, y, z;
					iss >> x >> y >> z;
					positions.push_back(glm::vec3(x, y, z));
				}
				else if (indice == "vn")
				{
					float x, y, z;
					iss >> x >> y >> z;
					normals.push_back(glm::vec3(x, y, z));
				}
				else if (indice == "vt")
				{
					float u, v;
					iss >> u >> v;
					textures.push_back(glm::vec2(u, v));
				}
				else if (indice == "f")
				{
					std::string f1, f2, f3;
					iss >> f1 >> f2 >> f3;

					std::string p, t, n;

					// for vertex 1
					Vertex v1;

					p = f1.substr(0, f1.find("/"));
					f1.erase(0, f1.find("/") + 1);

					t = f1.substr(0, f1.find("/"));
					f1.erase(0, f1.find("/") + 1);

					n = f1.substr(0, f1.find("/"));

					v1.Position = positions.at(std::stof(p) - 1);
					v1.Normal = normals.at(std::stof(n) - 1);
					v1.Texture = textures.at(std::stof(t) - 1);
					vertices.push_back(v1);

					// for vertex 2
					Vertex v2;

					p = f2.substr(0, f2.find("/"));
					f2.erase(0, f2.find("/") + 1);

					t = f2.substr(0, f2.find("/"));
					f2.erase(0, f2.find("/") + 1);

					n = f2.substr(0, f2.find("/"));

					v2.Position = positions.at(std::stof(p) - 1);
					v2.Normal = normals.at(std::stof(n) - 1);
					v2.Texture = textures.at(std::stof(t) - 1);
					vertices.push_back(v2);

					// for vertex 3
					Vertex v3;

					p = f3.substr(0, f3.find("/"));
					f3.erase(0, f3.find("/") + 1);

					t = f3.substr(0, f3.find("/"));
					f3.erase(0, f3.find("/") + 1);

					n = f3.substr(0, f3.find("/"));

					v3.Position = positions.at(std::stof(p) - 1);
					v3.Normal = normals.at(std::stof(n) - 1);
					v3.Texture = textures.at(std::stof(t) - 1);
					vertices.push_back(v3);
				}
			}
			infile.close();
		}
		else
		{
			std::cout << "Error opening file." << std::endl;
		}

		numVertices = vertices.size();
	}

	Object(float size, float height, bool transparent = false)
	{
		transparent = transparent;
		// 4 corners of the plane
		glm::vec3 p1(-size, height, -size);
		glm::vec3 p2(size, height, -size);
		glm::vec3 p3(size, height, size);
		glm::vec3 p4(-size, height, size);

		// Texture coordinates
		glm::vec2 t1(0.0f, 1.0f);
		glm::vec2 t2(1.0f, 1.0f);
		glm::vec2 t3(1.0f, 0.0f);
		glm::vec2 t4(0.0f, 0.0f);

		// Normal (pointing straight up)
		glm::vec3 n(0.0f, 1.0f, 0.0f);

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

	void makeObject(Shader shader, bool texture = true)
	{
		float *data = new float[8 * numVertices];
		for (int i = 0; i < numVertices; i++)
		{
			Vertex v = vertices.at(i);
			data[i * 8] = v.Position.x;
			data[i * 8 + 1] = v.Position.y;
			data[i * 8 + 2] = v.Position.z;

			data[i * 8 + 3] = v.Texture.x;
			data[i * 8 + 4] = v.Texture.y;

			data[i * 8 + 5] = v.Normal.x;
			data[i * 8 + 6] = v.Normal.y;
			data[i * 8 + 7] = v.Normal.z;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		// define VBO and VAO as active buffer and active vertex array
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);

		auto att_pos = glGetAttribLocation(shader.ID, "position");
		glEnableVertexAttribArray(att_pos);
		glVertexAttribPointer(att_pos, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)0);

		if (texture)
		{
			auto att_tex = glGetAttribLocation(shader.ID, "tex_coord");
			glEnableVertexAttribArray(att_tex);
			glVertexAttribPointer(att_tex, 2, GL_FLOAT, false, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		}

		auto att_col = glGetAttribLocation(shader.ID, "normal");
		glEnableVertexAttribArray(att_col);
		glVertexAttribPointer(att_col, 3, GL_FLOAT, false, 8 * sizeof(float), (void *)(5 * sizeof(float)));

		// desactive the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		delete[] data;
	}

	void draw()
	{
		if (transparent)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	}
};
#endif