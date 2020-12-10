#pragma once
// #include "Vertex.h"
#include "Face.h"
#include <vector>
#include <glm/glm.hpp>

class Mesh
{
public:
    Mesh();
    std::vector<glm::vec3> *GetVertices();
    std::vector<glm::vec2> *GetUVS();
    std::vector<glm::vec3> *GetNormals();
    std::vector<unsigned short> *GetIndexBuffer();
    Mesh *SetVertices(std::vector<glm::vec3>);
    Mesh *SetUVS(std::vector<glm::vec2>);
    Mesh *SetNormals(std::vector<glm::vec3>);
    Mesh *SetIndexBuffer(std::vector<unsigned short>);

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned short> index_buffer;
};