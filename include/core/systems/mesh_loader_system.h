// MeshLoaderSystem loads in all the specified meshes in all of the entities that have a StaticMeshComponent
// If there are entities that have a StaticMeshComponent and do not have their buffers or opengl data set
// then MeshLoaderSystem will do that with the specified path

#include "../../ecs/system/system.hpp"
#include "../components/static_mesh_component.h"
#include "../components/static_mesh_cache.h"
#include "../../ecs/world/world.hpp"
#include <memory>

class MeshLoaderSystem : public System
{
public:
    static std::shared_ptr<MeshLoaderSystem> Get();
    void Update();
    void DestroyEntity(Entity entity) override;
    static std::shared_ptr<MeshLoaderSystem> RegisterSystem();
    static LoadedStaticMesh *GetMesh(std::string mesh);

private:
    static void Import(std::string filepath, LoadedStaticMesh &component);
    static void IndexVBO(std::vector<glm::vec3> &in_vertices, std::vector<glm::vec2> &in_uvs, std::vector<glm::vec3> &in_normals, std::vector<unsigned short> &out_indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);

    static bool isLoaded(std::string model);
    static StaticMeshCache state;

    static void Every(void (*execute)(StaticMeshComponent &))
    {
        Signature signature;
        signature.set(World::Get()->GetComponentType<StaticMeshComponent>());
        for (Entity entity : World::Get()->GetEntities(signature))
        {
            auto &component = World::Get()->GetComponent<StaticMeshComponent>(entity);
            execute(component);
        }
    }
};