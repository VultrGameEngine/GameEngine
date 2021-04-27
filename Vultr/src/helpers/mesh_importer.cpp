#include <fstream>
#include <helpers/mesh_importer.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <helpers/path.h>

namespace Vultr
{

Mesh *MeshImporter::ImportMesh(const std::string &path)
{
    Mesh *mesh = new Mesh();
    Assimp::Importer importer;
    std::cout << "Loading component from path " << path << std::endl;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    const aiScene *scene = importer.ReadFile(
        Path::GetFullPath(path), aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                     aiProcess_JoinIdenticalVertices |
                                     aiProcess_SortByPType);
    if (!scene)
    {
        std::cout << "FAILED TO IMPORT MESH " << path << std::endl;
        return nullptr;
    }

    const aiMesh *imported_mesh = scene->mMeshes[0];
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
    for (unsigned int i = 0; i < imported_mesh->mNumVertices; i++)
    {
        const aiVector3D *pPos = &(imported_mesh->mVertices[i]);
        const aiVector3D *pNormal = &(imported_mesh->mNormals[i]);
        const aiVector3D *pTexCoord = imported_mesh->HasTextureCoords(0)
                                          ? &(imported_mesh->mTextureCoords[0][i])
                                          : &Zero3D;

        Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
                 glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                 glm::vec2(pTexCoord->x, pTexCoord->y));

        vertices.push_back(v);
    }
    for (unsigned int i = 0; i < imported_mesh->mNumFaces; i++)
    {
        const aiFace &face = imported_mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    mesh->Init(vertices, indices);
    return mesh;
}

Mesh *MeshImporter::InitQuad()
{
    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(-1, -1, 0)),
        Vertex(glm::vec3(-1, 1, 0)),
        Vertex(glm::vec3(1, -1, 0)),
        Vertex(glm::vec3(1, 1, 0)),
    };
    std::vector<unsigned short> indices = {
        0, 2, 1, 1, 2, 3,
    };
    Mesh *mesh = new Mesh();
    mesh->Init(vertices, indices);
    return mesh;
}

} // namespace Vultr
