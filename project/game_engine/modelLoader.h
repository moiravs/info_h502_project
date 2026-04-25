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

private:
    bool InitFromScene(const aiScene *pScene, const std::string &Filename);
    void InitMesh(unsigned int Index, const aiMesh *paiMesh);
    bool InitMaterials(const aiScene *pScene, const std::string &Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

public:
    struct MeshEntry
    {
        MeshEntry() = default;  // Add this
        ~MeshEntry() = default; // Add this
        bool Init(const std::vector<Vertex> &Vertices,
                  const std::vector<unsigned int> &Indices);

        GLuint VB;
        GLuint IB;
        unsigned int NumIndices;
        unsigned int MaterialIndex;
    };

    Mesh();

    ~Mesh();

    bool LoadMesh(const std::string &Filename);

    void Render();

    std::vector<MeshEntry> m_Entries; // One for each sub-mesh
    std::vector<Texture *> m_Textures;
};

#endif