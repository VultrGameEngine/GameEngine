#pragma once
#include <ecs/component/component_registry.hpp>
#include <platform/window/window.h>
#include <ecs/world/world.hpp>
#include <game.hpp>
#include <GLFW/glfw3.h>

namespace Vultr
{
    struct Engine
    {
        GLFWwindow *window;
        bool should_close = false;

        bool debug;
        Game *game;
        World *current_world;
        ComponentRegistry component_registry;
        SystemManager system_manager;
    };

    Engine &engine_global();

    World *get_current_world();
    void change_world(World *);

    void engine_init(Engine &e, bool debug);
    void engine_load_game(Engine &e, const char *path);
    void engine_load_game(Engine &e, Game *game);
    void engine_register_default_components(Engine &e);
    void engine_init_default_systems(Engine &e);
    void engine_init_game(Engine &e);
    void engine_update_game(Engine &e, float &last_time);
    double engine_get_time_elapsed(Engine &e);

#define WORLD_DOESNT_EXIST_ERROR(function_name) "Cannot call " #function_name " because world does not exist! Make sure you create a world before calling this method."

    template <typename T>
    ComponentType get_component_type()
    {
        return component_registry_get_component_type<T>(engine_global().component_registry);
    }

    template <typename T>
    T &entity_get_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->GetData(entity);
    }

    template <typename T>
    void entity_add_component(Entity entity, T component)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_add_component));
        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->InsertData(entity, component);

        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(), true);
        system_manager_entity_signature_changed(world_get_system_manager(world), entity, signature);

        system_manager_entity_signature_changed(engine_global().system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);
    }

    template <typename T>
    void entity_remove_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_remove_component));
        auto signature = get_entity_signature(world, entity);
        signature.set(get_component_type<T>(), false);
        system_manager_entity_signature_changed(world_get_system_manager(world), entity, signature);
        system_manager_entity_signature_changed(engine_global().system_manager, entity, signature);
        entity_manager_set_signature(world_get_entity_manager(world), entity, signature);

        component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->RemoveData(entity);
    }

    template <typename T>
    bool entity_has_component(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_has_component));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->HasComponent(entity);
    }

    template <typename T>
    T *entity_get_component_unsafe(Entity entity)
    {
        World *world = get_current_world();
        assert(world != nullptr && WORLD_DOESNT_EXIST_ERROR(entity_get_component_unsafe));
        return component_manager_get_component_array<T>(world_get_component_manager(world), get_component_type<T>())->GetDataUnsafe(entity);
    }

    Signature entity_get_signature(Entity entity);

    template <typename T>
    void register_component()
    {
        component_registry_register_component<T>(engine_global().component_registry, [](Entity entity) { entity_add_component(entity, T::Create()); });
    }

    template <typename T>
    bool is_component_registered()
    {
        ComponentType type = component_registry_get_component_type<T>(engine_global().component_registry);
        return component_registry_is_component_registered(engine_global().component_registry, type);
    }

    template <typename T>
    std::shared_ptr<T> world_register_system(Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        return system_manager_register_system<T>(world_get_system_manager(get_current_world()), signature, on_create_entity, on_destroy_entity, match_signature);
    }

    template <typename T>
    std::shared_ptr<T> register_global_system(Signature signature, OnCreateEntity on_create_entity = nullptr, OnDestroyEntity on_destroy_entity = nullptr, MatchSignature match_signature = nullptr)
    {
        return system_manager_register_system<T>(engine_global().system_manager, signature, on_create_entity, on_destroy_entity, match_signature);
    }

    template <typename T>
    void deregister_global_system()
    {
        return system_manager_deregister_system<T>(engine_global().system_manager);
    }

    template <typename T>
    std::shared_ptr<T> get_global_system_provider()
    {
        return system_manager_get_system_provider<T>(engine_global().system_manager);
    }

} // namespace Vultr
template <>
inline void RenderMember(const std::string &name, std::string &m)
{
    char *buf = new char[4096];
    strcpy(buf, m.c_str());
    ImGui::InputText(name.c_str(), buf, sizeof(char) * 4096);
    m = std::string(buf);
}

template <>
inline void RenderMember(const std::string &name, float &m)
{
    ImGui::DragFloat(name.c_str(), &m, 0.02f);
}

template <>
inline void RenderMember(const std::string &name, int &m)
{
    ImGui::DragInt(name.c_str(), &m);
}

template <>
inline void RenderMember(const std::string &name, double &m)
{
    ImGui::DragFloat(name.c_str(), (float *)&m, 0.02f);
}

template <>
inline void RenderMember(const std::string &name, bool &m)
{
    ImGui::Checkbox(name.c_str(), &m);
}

template <>
inline void RenderMember(const std::string &name, glm::vec3 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
}

template <>
inline void RenderMember(const std::string &name, glm::vec4 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.z, 0.02f);
}

template <>
inline void RenderMember(const std::string &name, glm::quat &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.w, 0.02f);
}

#ifndef WIN32
VULTR_REGISTER_COMPONENT(TransformComponent, position, rotation, scale);
VULTR_REGISTER_COMPONENT(StaticMeshComponent, path);
VULTR_REGISTER_COMPONENT(SkyBoxComponent, identifier, front, back, top, bottom, left, right);
VULTR_REGISTER_COMPONENT(LightComponent, some_param);
VULTR_REGISTER_COMPONENT(ControllerComponent, sens);
VULTR_REGISTER_COMPONENT(CameraComponent, enabled, fov, znear, zfar);
template <>
inline const char *Vultr::get_struct_name<MaterialComponent>()
{
    return ("MaterialComponent");
}
#endif

template <>
inline void RenderMember(const std::string &name, MaterialComponent::TexturePair &m)
{
    ImGui::PushID("texture");
    RenderMember(name, m.path);
    ImGui::PopID();
}

template <>
inline void RenderMember(const std::string &name, Color &m)
{
    ImGui::PushID(name.c_str());
    float *val = glm::value_ptr(m.value);
    ImGui::ColorEdit4(name.c_str(), val);
    ImGui::PopID();
}

template <>
inline void RenderComponent<MaterialComponent>(Entity entity)
{
    MaterialComponent *component = entity_get_component_unsafe<MaterialComponent>(entity);
    if (component == nullptr)
        return;
    if (ImGui::CollapsingHeader("MaterialComponent"))
    {
        ImGui::PushID("shader_path");
        RenderMember("shader_path", component->shader_path);
        ImGui::PopID();

        ImGui::PushID("textures");
        if (ImGui::CollapsingHeader("textures"))
        {
            RenderMember("textures", component->textures);
        }
        ImGui::PopID();

        ImGui::PushID("vec3s");
        if (ImGui::CollapsingHeader("vec3s"))
        {
            RenderMember("vec3s", component->vec3s);
        }
        ImGui::PopID();

        ImGui::PushID("vec4s");
        if (ImGui::CollapsingHeader("vec4s"))
        {
            RenderMember("vec4s", component->vec4s);
        }
        ImGui::PopID();

        ImGui::PushID("colors");
        if (ImGui::CollapsingHeader("colors"))
        {
            RenderMember("colors", component->colors);
        }
        ImGui::PopID();

        ImGui::PushID("ints");
        if (ImGui::CollapsingHeader("ints"))
        {
            RenderMember("ints", component->ints);
        }
        ImGui::PopID();

        ImGui::PushID("floats");
        if (ImGui::CollapsingHeader("floats"))
        {
            RenderMember("floats", component->floats);
        }
        ImGui::PopID();

        if (ImGui::Button("Remove"))
        {
            entity_remove_component<MaterialComponent>(entity);
        }
    }
}
