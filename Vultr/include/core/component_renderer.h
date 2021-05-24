#pragma once
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>
#include <ecs/component/component_array.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <type_info/type_info.h>
#include <vector>
#include <helpers/file.h>
#include <helpers/path.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

typedef void (*ComponentRender)(Vultr::Entity);

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
void RenderComponent(Vultr::Entity entity);

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
    ImGui::PushID((name + ".x").c_str());
    ImGui::Text("%s", name.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.x, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".y").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.y, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".z").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.z, 0.02f);
    ImGui::PopID();
}

template <>
inline void RenderMember(const std::string &name, glm::vec4 &m)
{
    ImGui::PushID((name + ".x").c_str());
    ImGui::Text("%s", name.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.x, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".y").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.y, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".z").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.z, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".w").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.w, 0.02f);
    ImGui::PopID();
}

template <>
inline void RenderMember(const std::string &name, glm::quat &m)
{
    ImGui::PushID((name + ".x").c_str());
    ImGui::Text("%s", name.c_str());
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.x, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".y").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.y, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".z").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.z, 0.02f);
    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID((name + ".w").c_str());
    ImGui::SetNextItemWidth(150);
    ImGui::DragFloat("", &m.w, 0.02f);
    ImGui::PopID();
}

template <>
inline void RenderMember(const std::string &name, Vultr::File &file)
{
    ImGui::Text("%s", name.c_str());
    ImGui::SameLine();
    if (ImGui::Button(Vultr::Path::get_shortened_resource_path(file.GetPath().string()).c_str()))
    {
        ImGuiFileDialog::Instance()->OpenDialog("FileChooser" + name, "Choose File", file.GetExtension(), Vultr::Path::get_resource_path());
    }

    if (ImGuiFileDialog::Instance()->Display("FileChooser" + name))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::cout << "Path " << filePath << std::endl;
            file.path = Vultr::Path::get_shortened_resource_path(filePathName);
        }
        ImGuiFileDialog::Instance()->Close();
    }
}

template <>
inline void RenderMember(const std::string &name, MaterialComponent::TexturePair &m)
{
    ImGui::PushID("texture");
    RenderMember(m.name, m.path);
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
