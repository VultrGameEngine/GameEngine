#pragma once
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

enum StaticMeshLoadingState
{
    notLoaded,
    loading,
    loaded
};

struct LoadedStaticMesh
{
    StaticMeshLoadingState loaded = notLoaded;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned short> indices;
};

struct StaticMeshCache
{
    std::unordered_map<std::string, LoadedStaticMesh> meshes;
};