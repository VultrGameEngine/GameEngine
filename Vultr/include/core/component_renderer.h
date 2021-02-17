#pragma once
#include <ecs/entity/entity.hpp>
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>

typedef void (*ComponentRender)(Entity);

#define _RENDER_MEMBER(T, x)                                                        \
    ImGui::PushID(typeid(T).name());                                                \
    RenderMember(#x, T->x);                                                         \
    ImGui::PopID();

#define VULTR_REGISTER_COMPONENT(T, ...)                                            \
    template <> inline void RenderComponent<T>(Entity entity)                       \
    {                                                                               \
        std::shared_ptr<T> component = entity.GetComponentUnsafe<T>();              \
        if (component == nullptr)                                                   \
            return;                                                                 \
        if (ImGui::CollapsingHeader(#T))                                            \
        {                                                                           \
            MAP(_RENDER_MEMBER, component, __VA_ARGS__);                            \
        }                                                                           \
    }

#define VULTR_REGISTER_MATERIAL(T, ...)                                             \
    template <> inline void RenderComponent<T>(Entity entity)                       \
    {                                                                               \
        std::shared_ptr<MaterialComponent> component =                              \
            entity.GetComponentUnsafe<MaterialComponent>();                         \
        if (component == nullptr)                                                   \
            return;                                                                 \
        std::shared_ptr<T> material = std::dynamic_pointer_cast<T>(component);      \
        if (material == nullptr)                                                    \
            return;                                                                 \
        if (ImGui::CollapsingHeader(#T))                                            \
        {                                                                           \
            MAP(_RENDER_MEMBER, material, __VA_ARGS__);                             \
        }                                                                           \
    }
template <typename T> void RenderComponent(Entity entity)
{
    ImGui::Text("Default Component Renderer");
}

template <typename T> void RenderMember(const std::string &name, T &m)
{
    ImGui::Text(("Unable to render type for member " + name).c_str());
}

template <> inline void RenderMember(const std::string &name, std::string &m)
{
    ImGui::InputText(name.c_str(), &m[0], sizeof(char) * 4096);
}

template <> inline void RenderMember(const std::string &name, float &m)
{
    ImGui::DragFloat(name.c_str(), &m, 0.02f);
}

template <> inline void RenderMember(const std::string &name, int &m)
{
    ImGui::DragInt(name.c_str(), &m);
}

template <> inline void RenderMember(const std::string &name, glm::vec3 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
}

template <> inline void RenderMember(const std::string &name, glm::quat &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.w, 0.02f);
}

VULTR_REGISTER_COMPONENT(StaticMeshComponent, m_path);
VULTR_REGISTER_COMPONENT(TransformComponent, position, rotation, scale);
VULTR_REGISTER_COMPONENT(CameraComponent, enabled, fov, znear, zfar);
VULTR_REGISTER_COMPONENT(ControllerComponent, sens);
VULTR_REGISTER_COMPONENT(LightComponent, some_param);
VULTR_REGISTER_COMPONENT(SkyBoxComponent, identifier, front, back, top, bottom, left,
                         right);
VULTR_REGISTER_MATERIAL(ForwardMaterial, texture_path);
VULTR_REGISTER_MATERIAL(SkyboxMaterial, identifier);
VULTR_REGISTER_MATERIAL(UnlitMaterial, color);
