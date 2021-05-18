#pragma once
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>
#include <ecs/component/component_array.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <type_info/type_info.h>
#include <vector>

using namespace Vultr;
typedef void (*ComponentRender)(Entity);

#define _RENDER_MEMBER(T, x)                                                                                                                                                                                          \
    ImGui::PushID(typeid(T).name());                                                                                                                                                                                  \
    RenderMember(#x, T->x);                                                                                                                                                                                           \
    ImGui::PopID();

#define VULTR_REGISTER_COMPONENT(T, ...)                                                                                                                                                                              \
    template <>                                                                                                                                                                                                       \
    inline void RenderComponent<T>(Entity entity)                                                                                                                                                                     \
    {                                                                                                                                                                                                                 \
        T *component = entity_get_component_unsafe<T>(entity);                                                                                                                                                        \
        if (component == nullptr)                                                                                                                                                                                     \
            return;                                                                                                                                                                                                   \
        if (ImGui::CollapsingHeader(#T))                                                                                                                                                                              \
        {                                                                                                                                                                                                             \
            MAP(_RENDER_MEMBER, component, __VA_ARGS__);                                                                                                                                                              \
            if (ImGui::Button("Remove"))                                                                                                                                                                              \
            {                                                                                                                                                                                                         \
                entity_remove_component<T>(entity);                                                                                                                                                                   \
            }                                                                                                                                                                                                         \
        }                                                                                                                                                                                                             \
    }                                                                                                                                                                                                                 \
    template <>                                                                                                                                                                                                       \
    inline const char *Vultr::get_struct_name<T>()                                                                                                                                                                    \
    {                                                                                                                                                                                                                 \
        return #T;                                                                                                                                                                                                    \
    }

template <typename T>
void RenderComponent(Entity entity)
{
    ImGui::Text("Default Component Renderer");
}

template <typename T>
void RenderMember(const std::string &name, T &m)
{
    ImGui::Text("Unable to render type for member %s", name);
}

template <template <typename...> class Map, typename V>
void RenderMember(const std::string &name, Map<std::string, V> &m)
{
    for (auto [k, v] : m)
    {
        RenderMember(k, m.at(k));
    }
}

template <typename V>
void RenderMember(const std::string &name, std::vector<V> &m)
{
    for (uint i = 0; i < m.size(); i++)
    {
        ImGui::PushID(i);
        RenderMember(name, m.at(i));
        ImGui::PopID();
    }
}
