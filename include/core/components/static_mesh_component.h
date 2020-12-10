#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct StaticMeshComponent
{
    bool loaded = false;
    std::string path;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned short> indices;
    unsigned int vao = 0;
    unsigned int ibo = 0;
    unsigned int vbo = 0;
    unsigned int nbo = 0;
};