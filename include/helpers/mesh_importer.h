#pragma once
#include <map>
#include <rendering/models/mesh.h>
#include <string>

namespace Brick3D
{
class MeshImporter
{
  public:
    static Mesh *ImportMesh(std::string path);
    static void IndexVBO(std::vector<glm::vec3> &in_vertices,
                         std::vector<glm::vec2> &in_uvs,
                         std::vector<glm::vec3> &in_normals,
                         std::vector<unsigned short> &out_indices,
                         std::vector<glm::vec3> &out_vertices,
                         std::vector<glm::vec2> &out_uvs,
                         std::vector<glm::vec3> &out_normals);
    static bool GetSimilarVertexIndex(
        Vertex &vertex, std::map<Vertex, unsigned short> &VertexToOutIndex,
        unsigned short &result);
};

} // namespace Brick3D
