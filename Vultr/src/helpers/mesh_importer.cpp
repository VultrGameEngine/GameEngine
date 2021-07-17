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

    Mesh *MeshImporter::import_mesh(const ModelSource &source)
    {
        Mesh *mesh = new Mesh();
        Assimp::Importer importer;
        std::cout << "Loading component from path " << source.path.string() << std::endl;
        std::vector<Vertex> vertices;
        std::vector<unsigned short> indices;

        const aiScene *scene = importer.ReadFile(source.path.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
        if (!scene)
        {
            std::cout << "FAILED TO IMPORT MESH " << source.path.string() << std::endl;
            return nullptr;
        }

        const aiMesh *imported_mesh = scene->mMeshes[0];
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
        for (unsigned int i = 0; i < imported_mesh->mNumVertices; i++)
        {
            const aiVector3D *pPos = &(imported_mesh->mVertices[i]);
            const aiVector3D *pNormal = &(imported_mesh->mNormals[i]);
            const aiVector3D *pTexCoord = imported_mesh->HasTextureCoords(0) ? &(imported_mesh->mTextureCoords[0][i]) : &Zero3D;

            Vertex v(Vec3(pPos->x, pPos->y, pPos->z), Vec3(pNormal->x, pNormal->y, pNormal->z), Vec2(pTexCoord->x, pTexCoord->y));

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

    Mesh *MeshImporter::init_quad()
    {
        std::vector<Vertex> vertices = {
            Vertex(Vec3(-1, -1, 0), Vec3(0), Vec2(0, 0)), // bottom left
            Vertex(Vec3(-1, 1, 0), Vec3(0), Vec2(0, 1)),  // top left
            Vertex(Vec3(1, -1, 0), Vec3(0), Vec2(1, 0)),  // bottom right
            Vertex(Vec3(1, 1, 0), Vec3(0), Vec2(1, 1)),   // top right
        };
        std::vector<unsigned short> indices = {
            1, 2, 3, 1, 0, 2,
        };
        Mesh *mesh = new Mesh();
        mesh->Init(vertices, indices);
        return mesh;
    }

    Mesh *MeshImporter::init_skybox()
    {
        std::vector<Vertex> vertices = {
            Vertex(Vec3(-1, -1, -1)), // 0
            Vertex(Vec3(1, -1, -1)),  // 1
            Vertex(Vec3(1, 1, -1)),   // 2
            Vertex(Vec3(1, 1, 1)),    // 3
            Vertex(Vec3(-1, 1, -1)),  // 4
            Vertex(Vec3(-1, 1, 1)),   // 5
            Vertex(Vec3(-1, -1, 1)),  // 6
            Vertex(Vec3(1, -1, 1)),   // 7
            Vertex(Vec3(1, 1, -1)),   // 8
        };
        std::vector<unsigned short> indices = {// Front
                                               1, 8, 4, 4, 0, 1,
                                               // Left
                                               6, 0, 4, 4, 5, 6,
                                               // Right
                                               1, 7, 3, 3, 8, 1,
                                               // Back
                                               6, 5, 3, 3, 7, 6,
                                               // Top
                                               4, 8, 3, 3, 5, 4,
                                               // Bottom
                                               0, 6, 1, 1, 6, 7};
        Mesh *mesh = new Mesh();
        mesh->Init(vertices, indices);
        return mesh;
    }

} // namespace Vultr
