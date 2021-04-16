#pragma once
#include <ecs/entity/entity.hpp>
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>
#include <ecs/component/component_array.hpp>

typedef void (*ComponentRender)(Entity);

#define _RENDER_MEMBER(T, x)                                                        \
    ImGui::PushID(typeid(T).name());                                                \
    RenderMember(#x, T->x);                                                         \
    ImGui::PopID();

#define VULTR_REGISTER_COMPONENT(T, ...)                                            \
    template <> inline void RenderComponent<T>(Entity entity)                       \
    {                                                                               \
        T *component = entity.GetComponentUnsafe<T>();                              \
        if (component == nullptr)                                                   \
            return;                                                                 \
        if (ImGui::CollapsingHeader(#T))                                            \
        {                                                                           \
            MAP(_RENDER_MEMBER, component, __VA_ARGS__);                            \
            if (ImGui::Button("Remove"))                                            \
            {                                                                       \
                entity.RemoveComponent<T>();                                        \
            }                                                                       \
        }                                                                           \
    }

template <typename T> void RenderComponent(Entity entity)
{
    ImGui::Text("Default Component Renderer");
}

template <typename T> void RenderMember(const std::string &name, T &m)
{
    ImGui::Text("Unable to render type for member %s", name.c_str());
}
