
#ifndef INFOH502_CPP_TERRAINMESH_H
#define INFOH502_CPP_TERRAINMESH_H
#include "mesh.h"


class TerrainMesh : public Mesh {
    float _width;
    int _imageWidth{};
    float _depth;
    int _imageHeight{};
    static glm::vec3 calculateNormal(int x, int z, int imageWidth, int imageHeight, const std::vector<float>& vertices);
protected:
    std::vector<Vertex> _vertices;
public:
    TerrainMesh(float width, float depth);
    static std::shared_ptr<TerrainMesh> terrainFromTexture(const std::string &texturePath, float width, float depth);
    [[nodiscard]] float getHeight(float x, float z) const;
    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getDepth() const;
    [[nodiscard]] const std::vector<Vertex>& getVertices() const;
};



#endif //INFOH502_CPP_TERRAINMESH_H
