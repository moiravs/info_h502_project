#include "mesh.h"

void Mesh::MeshEntry::init(const std::vector<Vertex> &Vertices, const std::vector<unsigned int> &Indices)
{
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, Indices.data(), GL_STATIC_DRAW);
}

Mesh::Mesh(const std::string &filename)
{
    // Release the previously loaded mesh (if it exists)
    clear();

    Assimp::Importer Importer;

    const aiScene *pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene)
    {
        initFromScene(pScene, filename);
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), Importer.GetErrorString());
    }
}

// Destructor
Mesh::~Mesh()
{
    clear(); // Ensure textures and buffers are cleaned up
}

void Mesh::initMesh(const unsigned int Index, const aiMesh *paiMesh)
{
    m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
    {
        Vertex v;
        v.Position = {paiMesh->mVertices[i].x, paiMesh->mVertices[i].y, paiMesh->mVertices[i].z};
        v.Normal = {paiMesh->mNormals[i].x, paiMesh->mNormals[i].y, paiMesh->mNormals[i].z};

        if (paiMesh->HasTextureCoords(0))
        {
            v.Texture = {paiMesh->mTextureCoords[0][i].x, paiMesh->mTextureCoords[0][i].y};
        }
        else
        {
            v.Texture = {0.0f, 0.0f};
        }
        Vertices.push_back(v);
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace &Face = paiMesh->mFaces[i];
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_Entries[Index].init(Vertices, Indices);
}

void Mesh::initMaterials(const aiScene *pScene, const std::string &Filename)
{
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir = (SlashIndex == std::string::npos) ? "." : Filename.substr(0, SlashIndex);

    for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial *pMaterial = pScene->mMaterials[i];
        m_Textures[i] = nullptr;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
            {
                std::string FullPath = Dir + "/" + Path.data;
                // Assuming your Texture class handles the GL loading
                m_Textures[i] = new Texture(FullPath);
            }
        }
    }
}

void Mesh::initFromScene(const aiScene *pScene, const std::string &Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    initMaterials(pScene, Filename);
}

void Mesh::clear()
{
}

std::shared_ptr<Mesh> Mesh::createPlane(const float size, const float height)
{
    auto mesh = std::make_shared<Mesh>();

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
    entry.init(planeVertices, planeIndices);
    entry.MaterialIndex = 0;

    mesh->m_Entries.push_back(entry);

    return mesh;
}
