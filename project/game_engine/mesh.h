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

#include "entity/camera.h"
#include "entity/light.h"
#include "entity/vertex.h"
#include "texture.h"

class Mesh
{
    void initFromScene(const aiScene *pScene, const std::string &Filename);
    void initMesh(unsigned int Index, const aiMesh *paiMesh);
    void initMaterials(const aiScene *pScene, const std::string &Filename);
    void clear();

#define INVALID_MATERIAL 0xFFFFFFFF

public:
    struct MeshEntry
    {
        MeshEntry() = default;  // Add this
        ~MeshEntry() = default; // Add this
        void init(const std::vector<Vertex> &Vertices,
                  const std::vector<unsigned int> &Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };
    Mesh()=default;
    explicit Mesh(const std::string &filename);
    ~Mesh();

    static std::shared_ptr<Mesh> createPlane(float size, float height);

    std::vector<MeshEntry> m_Entries; // One for each sub-mesh
    std::vector<Texture *> m_Textures;
};

#endif