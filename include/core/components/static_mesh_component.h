#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct StaticMeshComponent
{
    std::string path;
    unsigned int vao = 0;
    unsigned int ibo = 0;
    unsigned int vbo = 0;
    unsigned int nbo = 0;
};