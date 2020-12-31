// TextureLoader loads in all the specified meshes in all of the entities that
// have a TextureComponent If there are entities that have a TextureComponent
// and do not have their buffers or opengl data set then TextureLoader will do
// that with the specified path

#pragma once
#include "../../ecs/system/system.hpp"
#include "../../ecs/world/world.hpp"
#include "../components/sky_box_component.h"
#include "../components/texture_cache.h"
#include "../components/texture_component.h"
#include <glm/glm.hpp>
#include <memory>

class TextureLoaderSystem : public System {
public:
  static std::shared_ptr<TextureLoaderSystem> Get();
  void Update();
  static std::shared_ptr<TextureLoaderSystem> RegisterSystem();
  static void Import(std::string path, LoadedTexture &texture);
  static void ImportSkybox(std::vector<std::string> paths,
                           LoadedTexture &texture);
  static LoadedTexture *GetTexture(std::string mesh);

private:
  static bool isLoaded(std::string texture);
  static TextureCache state;
  Signature signature;
};
