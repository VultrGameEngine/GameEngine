#pragma once
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "../../helpers/loading_state.h"

struct LoadedStaticMesh
{
    LoadingState loaded = notLoaded;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned short> indices;
};

struct StaticMeshCache
{
    std::unordered_map<std::string, LoadedStaticMesh> meshes;
};