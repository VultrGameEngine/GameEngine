#include <ecs/world/world.hpp>
#include <yaml-cpp/yaml.h>
#include <helpers/file.h>

std::shared_ptr<World> World::current_world = nullptr;

std::shared_ptr<World> World::Get()
{
    return current_world;
}

void World::ChangeWorld(std::shared_ptr<World> world)
{
    if (current_world != nullptr)
    {
        for (EntityID id : current_world->entity_manager->living_entites)
        {
            Entity entity(id);
            Vultr::Engine::GetGlobalSystemManager().EntityDestroyed(entity);
        }
    }

    current_world = world;
    for (EntityID id : current_world->entity_manager->living_entites)
    {
        Entity entity(id);
        Signature signature = current_world->entity_manager->GetSignature(entity);
        Vultr::Engine::GetGlobalSystemManager().EntitySignatureChanged(id,
                                                                       signature);
    }
    world->system_manager = std::make_unique<SystemManager>();
}

std::shared_ptr<World> World::Init()
{
    std::shared_ptr<World> world = std::make_shared<World>();
    world->component_manager = std::make_unique<ComponentManager>();
    world->entity_manager = std::make_unique<EntityManager>();
    world->system_manager = std::make_unique<SystemManager>();
    return world;
}

void World::ExportWorldEditor(const std::string &path, std::shared_ptr<World> world)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value;
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    for (Entity entity : world->entity_manager->living_entites)
    {
    }
    // m_Scene->m_Registry.each([&](auto entityID) {
    //     Entity entity = {entityID, m_Scene.get()};
    //     if (!entity)
    //         return;

    //     SerializeEntity(out, entity);
    // });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(path);
    fout << out.c_str();
}

void World::ExportWorld(const std::string &path, std::shared_ptr<World> world)
{
    std::ofstream os(path);
    // cereal::BinaryOutputArchive oarchive(os);

    // oarchive(world);
}

std::shared_ptr<World> World::ImportWorld(const std::string &path)
{
    std::shared_ptr<World> world;
    std::ifstream is(path);
    // cereal::BinaryInputArchive iarchive(is);

    // iarchive(world);
    return world;
}

Entity World::CreateEntity()
{
    return entity_manager->CreateEntity();
}

void World::DestroyEntity(Entity entity)
{
    entity_manager->DestroyEntity(entity);

    system_manager->EntityDestroyed(entity);

    Vultr::Engine::GetGlobalSystemManager().EntityDestroyed(entity);

    component_manager->EntityDestroyed(entity);
}

std::set<Entity> World::GetEntities(Signature signature)
{
    return entity_manager->GetEntities(signature);
}

Signature World::GetSignature(Entity entity)
{
    return entity_manager->GetSignature(entity);
}
