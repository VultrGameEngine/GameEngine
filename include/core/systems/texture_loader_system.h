// TextureLoader loads in all the specified meshes in all of the entities that have a TextureComponent
// If there are entities that have a TextureComponent and do not have their buffers or opengl data set
// then TextureLoader will do that with the specified path

#include "../../ecs/system/system.hpp"
#include "../components/texture_component.h"
#include "../components/sky_box_component.h"
#include "../components/texture_cache.h"
#include "../../ecs/coordinator/coordinator.hpp"
#include <glm/glm.hpp>
#include <memory>

class TextureLoaderSystem : public System
{
public:
    static std::shared_ptr<TextureLoaderSystem> Get();
    void Update();
    void DestroyEntity(Entity entity) override;
    static std::shared_ptr<TextureLoaderSystem> RegisterSystem();
    static void Import(std::string path, LoadedTexture &texture);
    static void ImportSkybox(std::vector<std::string> paths, LoadedTexture &texture);
    static LoadedTexture *GetTexture(std::string mesh);

private:
    static bool isLoaded(std::string texture);
    static TextureCache state;
};