#include "../../include/models/Mesh.h"
#include "../../include/models/Face.h"
#include <vector>

Mesh::Mesh()
{
}

std::vector<glm::vec3> *Mesh::GetVertices()
{
    return &this->vertices;
}
std::vector<glm::vec2> *Mesh::GetUVS()
{
    return &this->uvs;
}
std::vector<glm::vec3> *Mesh::GetNormals()
{
    return &this->normals;
}
std::vector<unsigned short> *Mesh::GetIndexBuffer()
{
    return &this->index_buffer;
}

Mesh *Mesh::SetVertices(std::vector<glm::vec3> vertices)
{
    this->vertices = vertices;
    return this;
}
Mesh *Mesh::SetUVS(std::vector<glm::vec2> uvs)
{
    this->uvs = uvs;
    return this;
}
Mesh *Mesh::SetNormals(std::vector<glm::vec3> normals)
{
    this->normals = normals;
    return this;
}
Mesh *Mesh::SetIndexBuffer(std::vector<unsigned short> buffer)
{
    this->index_buffer = buffer;
    return this;
}