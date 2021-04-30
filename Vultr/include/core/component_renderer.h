#pragma once
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>
#include <ecs/component/component_array.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Vultr;
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

template <template <typename...> class Map, typename V>
void RenderMember(const std::string &name, Map<std::string, V> &m)
{
    for (auto [k, v] : m)
    {
        RenderMember(k, m.at(k));
    }
}

template <typename V> void RenderMember(const std::string &name, std::vector<V> &m)
{
    for (uint i = 0; i < m.size(); i++)
    {
        ImGui::PushID(i);
        RenderMember(name, m.at(i));
        ImGui::PopID();
    }
}

template <> inline void RenderMember(const std::string &name, std::string &m)
{
    char *buf = new char[4096];
    strcpy(buf, m.c_str());
    ImGui::InputText(name.c_str(), buf, sizeof(char) * 4096);
    m = std::string(buf);
}

template <> inline void RenderMember(const std::string &name, float &m)
{
    ImGui::DragFloat(name.c_str(), &m, 0.02f);
}

template <> inline void RenderMember(const std::string &name, int &m)
{
    ImGui::DragInt(name.c_str(), &m);
}

template <> inline void RenderMember(const std::string &name, double &m)
{
    ImGui::DragFloat(name.c_str(), (float *)&m, 0.02f);
}

template <> inline void RenderMember(const std::string &name, bool &m)
{
    ImGui::Checkbox(name.c_str(), &m);
}

template <> inline void RenderMember(const std::string &name, glm::vec3 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
}

template <> inline void RenderMember(const std::string &name, glm::vec4 &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.z, 0.02f);
}

template <> inline void RenderMember(const std::string &name, glm::quat &m)
{
    ImGui::DragFloat((name + ".x").c_str(), &m.x, 0.02f);
    ImGui::DragFloat((name + ".y").c_str(), &m.y, 0.02f);
    ImGui::DragFloat((name + ".z").c_str(), &m.z, 0.02f);
    ImGui::DragFloat((name + ".w").c_str(), &m.w, 0.02f);
}

#ifndef WIN32
VULTR_REGISTER_COMPONENT(TransformComponent, position, rotation, scale);
VULTR_REGISTER_COMPONENT(StaticMeshComponent, m_path);
VULTR_REGISTER_COMPONENT(SkyBoxComponent, identifier, front, back, top, bottom, left,
                         right);
VULTR_REGISTER_COMPONENT(LightComponent, some_param);
VULTR_REGISTER_COMPONENT(ControllerComponent, sens);
VULTR_REGISTER_COMPONENT(CameraComponent, enabled, fov, znear, zfar);

template <>
inline void RenderMember(const std::string &name, MaterialComponent::TexturePair &m)
{
    ImGui::PushID("texture");
    RenderMember(name, m.path);
    ImGui::PopID();
}

template <> inline void RenderMember(const std::string &name, Color &m)
{
    ImGui::PushID(name.c_str());
    float *val = glm::value_ptr(m.value);
    ImGui::ColorEdit4(name.c_str(), val);
    ImGui::PopID();
}

template <> inline void RenderComponent<MaterialComponent>(Entity entity)
{
    MaterialComponent *component = entity.GetComponentUnsafe<MaterialComponent>();
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
            entity.RemoveComponent<MaterialComponent>();
        }
    }
}

#endif
