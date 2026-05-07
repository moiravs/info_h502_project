#ifndef Mesh_H
#define Mesh_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <memory>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h>      // Output data structure
#include <assimp/postprocess.h>
#include <vector>

#include "../entity/camera.h"
#include "../entity/light/light.h"
#include "../entity/vertex.h"
#include "../texture.h"

struct MeshEntry
{
    MeshEntry() = default;  // Add this
    ~MeshEntry() = default; // Add this
    void init(const std::vector<Vertex> &vertices,
              const std::vector<unsigned int> &indices);

    GLuint VB;
    GLuint IB;
    glm::vec3 minBound;
    glm::vec3 maxBound;
    int numIndices;
    unsigned int materialIndex;
};

class Mesh
{
    void initFromScene(const aiScene *pScene, const std::string &Filename);
    void initMesh(unsigned int Index, const aiMesh *paiMesh);
    void initMaterials(const aiScene *pScene, const std::string &Filename);
    void clear();
#define INVALID_MATERIAL 0xFFFFFFFF

    glm::vec3 minBound{};
    glm::vec3 maxBound{};
    void updateBounds();
    std::vector<MeshEntry> m_Entries; // One for each sub-mesh

public:
    Mesh()=default;
    explicit Mesh(const std::string &filename);
    ~Mesh();

    static std::shared_ptr<Mesh> createPlane(float size, float height);
    [[nodiscard]] std::pair<glm::vec3, glm::vec3> getBounds() const;
    void addEntry(const MeshEntry &entry);
    [[nodiscard]] const std::vector<MeshEntry>& getEntries() const;

    std::vector<Texture *> m_Textures;
};

#endif