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
    Mesh MeshImporter::import_mesh(const ModelSource &source)
    {
        Assimp::Importer importer;
        std::cout << "Loading component from path " << source.path.string() << std::endl;

        const aiScene *scene = importer.ReadFile(source.path.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
        if (!scene)
        {
            std::cout << "FAILED TO IMPORT MESH " << source.path.string() << std::endl;
            return {};
        }

        const aiMesh *imported_mesh = scene->mMeshes[0];
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

        Vertex *vertices = new Vertex[imported_mesh->mNumVertices];
        for (size_t i = 0; i < imported_mesh->mNumVertices; i++)
        {
            const aiVector3D *pPos = &(imported_mesh->mVertices[i]);
            const aiVector3D *pNormal = &(imported_mesh->mNormals[i]);
            const aiVector3D *pTexCoord = imported_mesh->HasTextureCoords(0) ? &(imported_mesh->mTextureCoords[0][i]) : &Zero3D;
            const aiVector3D *pTangent = &(imported_mesh->mTangents[i]);

            vertices[i] = Vertex(Vec3(pPos->x, pPos->y, pPos->z), Vec3(pNormal->x, pNormal->y, pNormal->z), Vec2(pTexCoord->x, pTexCoord->y), Vec3(pTangent->x, pTangent->y, pTangent->z));
        }

        unsigned short *indices = new unsigned short[imported_mesh->mNumFaces * 3];
        size_t index = 0;
        for (size_t i = 0; i < imported_mesh->mNumFaces; i++)
        {
            const aiFace &face = imported_mesh->mFaces[i];
            assert(face.mNumIndices == 3);
            indices[index] = face.mIndices[0];
            indices[index + 1] = face.mIndices[1];
            indices[index + 2] = face.mIndices[2];
            index += 3;
        }

        return new_mesh(vertices, imported_mesh->mNumVertices, indices, imported_mesh->mNumFaces * 3);
    }

    Mesh MeshImporter::init_quad()
    {
        // TODO: Add bitangent calculations
        Vertex vertices[] = {
            Vertex(Vec3(-1, -1, 0), Vec3(0), Vec2(0, 0)), // bottom left
            Vertex(Vec3(-1, 1, 0), Vec3(0), Vec2(0, 1)),  // top left
            Vertex(Vec3(1, -1, 0), Vec3(0), Vec2(1, 0)),  // bottom right
            Vertex(Vec3(1, 1, 0), Vec3(0), Vec2(1, 1)),   // top right
        };
        u16 indices[] = {
            1, 2, 3, 1, 0, 2,
        };
        return new_mesh(vertices, 4, indices, 6);
    }

    Mesh MeshImporter::init_skybox()
    {
        Vertex vertices[] = {
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
        u16 indices[] = {// Front
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

        return new_mesh(vertices, 9, indices, 36);
    }

} // namespace Vultr
