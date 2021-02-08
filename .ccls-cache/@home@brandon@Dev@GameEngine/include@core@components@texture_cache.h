#pragma once
#include "../../helpers/loading_state.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

struct LoadedTexture {
  unsigned int id = 0;
  LoadingState loaded = notLoaded;

  // Bind the texture
  void Bind(GLenum slot) {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, id);
  }

  // Unbind the texture
  void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
};

struct TextureCache {
  std::unordered_map<std::string, LoadedTexture> textures;
};
