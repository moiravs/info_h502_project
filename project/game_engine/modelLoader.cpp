#include "modelLoader.h"

bool Mesh::MeshEntry::Init(const std::vector<Vertex> &Vertices, const std::vector<unsigned int> &Indices)
{
    NumIndices = Indices.size();

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, Indices.data(), GL_STATIC_DRAW);

    return true;
}

Mesh::Mesh()
{
    // Initialize any members if necessary
}

// Destructor
Mesh::~Mesh()
{
    Clear(); // Ensure textures and buffers are cleaned up
}

void Mesh::InitMesh(unsigned int Index, const aiMesh *paiMesh)
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

    m_Entries[Index].Init(Vertices, Indices);
}

bool Mesh::InitMaterials(const aiScene *pScene, const std::string &Filename)
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
    return true;
}

bool Mesh::InitFromScene(const aiScene *pScene, const std::string &Filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_Entries.size(); i++)
    {
        const aiMesh *paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh);
    }

    return InitMaterials(pScene, Filename);
}

bool Mesh::LoadMesh(const std::string &Filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene *pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if (pScene)
    {
        Ret = InitFromScene(pScene, Filename);
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

    return Ret;
}

void Mesh::Clear()
{
}