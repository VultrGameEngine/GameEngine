#pragma once
#include <imgui/imgui.h>
#include <macros/map.h>
#include <core/core_components.h>
#include <ecs/component/component_array.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <type_info/type_info.h>
#include <vector>
#include <filesystem/file.h>
#include <ImGuiFileDialog/ImGuiFileDialog.h>

namespace Vultr
{

    struct Engine;
    typedef void (*ComponentRender)(Vultr::Engine *e, Vultr::Entity);

    struct RenderMemberResult
    {
        bool finished_editing = false;
        bool started_editing = false;
        void operator=(const RenderMemberResult &other)
        {
            if (other.finished_editing)
                finished_editing = true;
            if (other.started_editing)
                started_editing = true;
        }
    };
    RenderMemberResult was_edited();

#define _RENDER_MEMBER(e, T, x)                                                                                                                                                                                       \
    ImGui::PushID(typeid(T).name());                                                                                                                                                                                  \
    RenderMember(#x, T->x);                                                                                                                                                                                           \
    ImGui::PopID();

#define VULTR_REGISTER_COMPONENT(T, ...)                                                                                                                                                                              \
    template <>                                                                                                                                                                                                       \
    inline void RenderComponent<T>(Engine * e, Entity entity)                                                                                                                                                         \
    {                                                                                                                                                                                                                 \
        T *component = entity_get_component_unsafe<T>(e, entity);                                                                                                                                                     \
        if (component == nullptr)                                                                                                                                                                                     \
            return;                                                                                                                                                                                                   \
        if (ImGui::CollapsingHeader(#T))                                                                                                                                                                              \
        {                                                                                                                                                                                                             \
            MAP(_RENDER_MEMBER, e, component, __VA_ARGS__);                                                                                                                                                           \
            if (ImGui::Button("Remove"))                                                                                                                                                                              \
            {                                                                                                                                                                                                         \
                entity_remove_component<T>(e, entity);                                                                                                                                                                \
            }                                                                                                                                                                                                         \
        }                                                                                                                                                                                                             \
    }                                                                                                                                                                                                                 \
    template <>                                                                                                                                                                                                       \
    inline const char *Vultr::get_struct_name<T>()                                                                                                                                                                    \
    {                                                                                                                                                                                                                 \
        return #T;                                                                                                                                                                                                    \
    }

    template <typename T>
    void RenderComponent(Engine *e, Vultr::Entity entity);

    template <typename T>
    RenderMemberResult RenderMember(const std::string &name, T &m)
    {
        ImGui::Text("Unable to render type for member %s", name.c_str());
        return {false, false};
    }

    template <template <typename...> class Map, typename V>
    RenderMemberResult RenderMember(const std::string &name, Map<std::string, V> &m)
    {
        RenderMemberResult res;
        for (auto [k, v] : m)
        {
            res = RenderMember(k, m.at(k));
        }
        return res;
    }

    template <typename V>
    RenderMemberResult RenderMember(const std::string &name, std::vector<V> &m)
    {
        RenderMemberResult res;
        for (uint i = 0; i < m.size(); i++)
        {
            ImGui::PushID(i);
            res = RenderMember(name, m.at(i));
            ImGui::PopID();
        }
        return res;
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, std::string &m)
    {
        char *buf = new char[4096];
        strcpy(buf, m.c_str());
        ImGui::InputText(name.c_str(), buf, sizeof(char) * 4096);
        m = std::string(buf);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, f32 &m)
    {
        ImGui::DragFloat(name.c_str(), &m, 0.02f);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, f64 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_Double, &m, 0.02f);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, u32 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_U32, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, u16 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_U16, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, u8 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_U8, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, s32 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_S32, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, s16 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_S16, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, s8 &m)
    {
        ImGui::DragScalar(name.c_str(), ImGuiDataType_S16, &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, bool &m)
    {
        ImGui::Checkbox(name.c_str(), &m);
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vec3 &m)
    {
        RenderMemberResult res;
        ImGui::PushID((name + ".x").c_str());
        ImGui::Text("%s", name.c_str());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.x, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".y").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.y, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".z").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.z, 0.02f);
        ImGui::PopID();
        res = was_edited();
        return res;
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vec4 &m)
    {
        RenderMemberResult res;
        ImGui::PushID((name + ".x").c_str());
        ImGui::Text("%s", name.c_str());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.x, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".y").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.y, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".z").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.z, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".w").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.w, 0.02f);
        ImGui::PopID();
        res = was_edited();
        return res;
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Quat &m)
    {
        RenderMemberResult res;
        ImGui::PushID((name + ".x").c_str());
        ImGui::Text("%s", name.c_str());
        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.x, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".y").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.y, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".z").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.z, 0.02f);
        ImGui::SameLine();
        ImGui::PopID();
        res = was_edited();

        ImGui::PushID((name + ".w").c_str());
        ImGui::SetNextItemWidth(150);
        ImGui::DragFloat("", &m.w, 0.02f);
        ImGui::PopID();
        res = was_edited();
        return res;
    }

    // TODO: Finish this
    template <const char *const extensions[]>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::File<extensions> *file)
    {
        ImGui::PushID(name.c_str());
        ImGui::Text("%s", name.c_str());
        ImGui::SameLine();
        RenderMemberResult res;
        if (ImGui::Button("##", ImVec2(300, 150)))
        {
            char *expected_extensions = file->expected_extensions_string();
            ImGuiFileDialog::Instance()->OpenDialog("FileChooser" + name, "Choose File", expected_extensions, "");
            free(expected_extensions);
            res.started_editing = true;
        }

        if (ImGui::BeginDragDropTarget())
        {
            auto *payload = ImGui::GetDragDropPayload();
            if (payload != nullptr && payload->IsDataType("File"))
            {
                GenericFile *payload_file = static_cast<GenericFile *>(payload->Data);
                // if (file_extension_matches(file, *payload_file))
                // {
                //     payload = ImGui::AcceptDragDropPayload("File");
                //     if (payload != nullptr)
                //     {
                //         payload_file = static_cast<GenericFile *>(payload->Data);
                //         file.path = Vultr::file_get_relative_path(*payload_file);
                //         ImGui::EndDragDropTarget();
                //         res.started_editing = true;
                //         res.finished_editing = true;
                //     }
                // }
            }
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 270);
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 270);
        ImGui::Text("%s", fbasename(&file));
        ImGui::PopTextWrapPos();

        if (ImGuiFileDialog::Instance()->Display("FileChooser" + name))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                // std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                // std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // file.path = Vultr::file_get_relative_path(Vultr::File(filePathName));
                // std::cout << "Path " << file.path << std::endl;
            }
            ImGuiFileDialog::Instance()->Close();
            res.finished_editing = true;
        }
        ImGui::PopID();
        return res;
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::TextureSource &m)
    {
        return RenderMember<Vultr::TextureSource>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::ModelSource &m)
    {
        return RenderMember<Vultr::ModelSource>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::HeaderFile &m)
    {
        return RenderMember<Vultr::HeaderFile>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::SourceFile &m)
    {
        return RenderMember<Vultr::SourceFile>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::HeaderAndSourceFile &m)
    {
        return RenderMember<Vultr::HeaderAndSourceFile>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Vultr::ShaderSource &m)
    {
        return RenderMember<Vultr::ShaderSource>(name, m);
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, MaterialComponent::TextureResource &m)
    {
        ImGui::PushID("texture");
        auto res = RenderMember(m.location, m.file);
        ImGui::PopID();
        return res;
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, Color &m)
    {
        ImGui::PushID(name.c_str());
        m.value /= Vec4(255);
        float *val = glm::value_ptr(m.value);
        ImGui::ColorEdit4(name.c_str(), val);
        m.value *= Vec4(255);
        ImGui::PopID();
        return was_edited();
    }

    template <>
    inline RenderMemberResult RenderMember(const std::string &name, MaterialUniform &m)
    {
        if (m.type == MaterialUniform::EMPTY)
            return {false, false};

        ImGui::PushID(name.c_str());
        auto &data = m.data;
        switch (m.type)
        {
            case MaterialUniform::BOOL:
                RenderMember<bool>(name.c_str(), data.u_bool);
                break;
            case MaterialUniform::U32:
                RenderMember<u32>(name.c_str(), data.u_u32);
                break;
            case MaterialUniform::S32:
                RenderMember<s32>(name.c_str(), data.u_s32);
                break;
            case MaterialUniform::F32:
                RenderMember<f32>(name.c_str(), data.u_f32);
                break;
            case MaterialUniform::VEC2:
                RenderMember<Vec2>(name.c_str(), data.u_vec2);
                break;
            case MaterialUniform::VEC3:
                RenderMember<Vec3>(name.c_str(), data.u_vec3);
                break;
            case MaterialUniform::VEC4:
                RenderMember<Vec4>(name.c_str(), data.u_vec4);
                break;
            case MaterialUniform::COLOR:
                RenderMember<Color>(name.c_str(), data.u_color);
                break;
            case MaterialUniform::MAT4:
                // TODO: Add support for mat4s
                break;
            case MaterialUniform::EMPTY:
                // Nothing to do here, as this is the default
                break;
        }
        ImGui::PopID();
        return was_edited();
    }
} // namespace Vultr
