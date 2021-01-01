#include "../../../include/core/systems/texture_loader_system.h"
#include "../../../include/ecs/world/world.hpp"
#include "../../../include/helpers/loading_state.h"
#include "../../../vendor/stb_image/stb_image.h"
#include "GLFW/glfw3.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string.h>
#include <string>
#include <thread>
#include <vector>

TextureCache TextureLoaderSystem::state = {};

std::shared_ptr<TextureLoaderSystem> TextureLoaderSystem::Get() {
  static std::shared_ptr<TextureLoaderSystem> instance;
  if (instance == 0) {

    instance = RegisterSystem();
  }
  return instance;
}

std::shared_ptr<TextureLoaderSystem> TextureLoaderSystem::RegisterSystem() {
  std::shared_ptr<TextureLoaderSystem> ptr =
      World::RegisterSystem<TextureLoaderSystem>();
  ptr->signature.set(World::GetComponentType<TextureComponent>());
  World::SetSignature<TextureLoaderSystem>(ptr->signature);

  return ptr;
}

void TextureLoaderSystem::Update() {
  for (Entity entity : entities) {
    auto &component = World::GetComponent<TextureComponent>(entity);
    if (!isLoaded(component.diffuse)) {
      state.textures[component.diffuse] = {};
      Import(component.diffuse, state.textures[component.diffuse]);
    }
    if (!isLoaded(component.specular)) {
      state.textures[component.specular] = {};
      Import(component.specular, state.textures[component.specular]);
    }
  }
  Signature skybox_signature;
  skybox_signature.set(World::GetComponentType<SkyBoxComponent>());
  for (Entity entity : World::GetEntities(skybox_signature)) {
    auto &component = World::GetComponent<SkyBoxComponent>(entity);
    if (!isLoaded(component.identifier)) {
      state.textures[component.identifier] = {};
      ImportSkybox(component.GetPaths(), state.textures[component.identifier]);
    }
  }
}
LoadedTexture *TextureLoaderSystem::GetTexture(std::string path) {
  if (isLoaded(path)) {
    return &(state.textures[path]);
  } else {
    return nullptr;
  }
}

bool TextureLoaderSystem::isLoaded(std::string texture) {
  return state.textures.find(texture) != state.textures.end();
}

void TextureLoaderSystem::Import(std::string path, LoadedTexture &texture) {
  texture.loaded = loading;
  stbi_set_flip_vertically_on_load(1);
  int width;
  int height;
  int bpp;
  unsigned char *buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, buffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  if (buffer)
    stbi_image_free(buffer);

  texture.loaded = loaded;
}
void TextureLoaderSystem::ImportSkybox(std::vector<std::string> paths,
                                       LoadedTexture &texture) {
  texture.loaded = loading;
  stbi_set_flip_vertically_on_load(0);
  glGenTextures(1, &texture.id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < paths.size(); i++) {
    unsigned char *data =
        stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed to load at path: " << paths[i]
                << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  texture.loaded = loaded;
}
