#pragma once
#include "../../helpers/loading_state.h"
#include "../../rendering/models/vertex.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

struct LoadedStaticMesh {
  LoadingState loaded = notLoaded;
  std::vector<Renderer::Vertex> vertices;
  std::vector<unsigned short> indices;

  inline void AddVertices(std::vector<glm::vec3> positions,
                          std::vector<glm::vec2> uvs,
                          std::vector<glm::vec3> normals,
                          std::vector<unsigned short> in_indices) {
    for (int i = 0; i < positions.size(); i++) {
      vertices.push_back(
          Renderer::Vertex(positions.at(i), normals.at(i), uvs.at(i)));
    }
    indices = in_indices;
  }

  unsigned int vbo = 0;
  unsigned int vao = 0;
  unsigned int ibo = 0;
};

struct StaticMeshCache {
  std::unordered_map<std::string, LoadedStaticMesh> meshes;
};
