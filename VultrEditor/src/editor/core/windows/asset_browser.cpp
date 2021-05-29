#include <editor/core/windows/asset_browser.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <types/types.hpp>
#include <fonts/fork_awesome.h>

using namespace Vultr;
static Texture *get_texture_from_file(const File &file)
{
    auto extension = file_get_extension_type(file);
    auto *e = Editor::Get();
    switch (extension)
    {
    case File::SHADER: {
        return e->shader_icon;
    }
    case File::HEADER:
    case File::SOURCE:
        return e->c_icon;
    case File::TEXTURE:
        return e->image_icon;
    case File::MODEL:
        return e->model_icon;
    case File::VULTR:
        return e->vultr_icon;
    default:
        return e->file_icon;
    }
}

void AssetBrowser::Render()
{
    auto &current_directory = Editor::Get()->current_directory;
    static std::vector<File> files;
    static std::vector<Directory> sub_directories;
    static s32 selected = -1;
    files = directory_get_files(current_directory);
    sub_directories = directory_get_sub_directories(current_directory);
    ImGui::Begin("Asset Browser");
    s32 num_cols = ImGui::GetWindowWidth() / 200 - 2;
    if (num_cols < 1)
        num_cols = 1;
    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 150);
    if (ImGui::BeginTable("table1", num_cols))
    {
        s32 index = 0;

        while (index < files.size() + sub_directories.size())
        {
            ImGui::TableNextRow();
            for (int column = 0; column < num_cols; column++)
            {
                if (index >= files.size() + sub_directories.size())
                    break;
                std::string label;
                bool is_directory = index < sub_directories.size();
                if (!is_directory)
                {
                    label = file_get_name(files[index - sub_directories.size()]);
                }
                else
                {
                    label = directory_get_name(sub_directories[index]);
                }
                ImGui::TableSetColumnIndex(column);
                ImGui::PushID(index);
                if (ImGui::Selectable("##", selected == index, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(200, 200)))
                {
                    selected = index;
                }
                if (ImGui::BeginDragDropSource())
                {
                    void *payload = is_directory ? (void *)new Directory(sub_directories[index]) : (void *)new File(files[index - sub_directories.size()]);
                    ImGui::SetDragDropPayload(is_directory ? "Directory" : "File", payload, sizeof(Vultr::File));
                    free(payload);
                    auto *texture = is_directory ? Editor::Get()->folder_icon : get_texture_from_file(files[index - sub_directories.size()]);
                    texture->Bind(GL_TEXTURE0);
                    ImGui::Image((void *)(intptr_t)texture->GetID(), ImVec2(125, 125));
                    ImGui::Text("%s", label.c_str());
                    ImGui::EndDragDropSource();
                }
                if (is_directory && ImGui::BeginDragDropTarget())
                {
                    const auto *payload = ImGui::AcceptDragDropPayload("File");
                    if (payload != nullptr)
                    {
                        auto *file = static_cast<Vultr::File *>(payload->Data);
                        move_file(*file, sub_directories[index]);
                        current_directory.cached_files = false;
                        selected = -1;
                        ImGui::PopID();
                        ImGui::EndTable();
                        ImGui::End();
                        return;
                    }
                    ImGui::EndDragDropTarget();
                }
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    if (is_directory)
                    {
                        current_directory = sub_directories[index];
                        current_directory.cached_files = false;
                        selected = -1;
                        ImGui::PopID();
                        ImGui::EndTable();
                        ImGui::End();
                        return;
                    }
                    else
                    {
                        auto &file = files[index - sub_directories.size()];
                        if (file_get_extension_type(file) == File::VULTR)
                        {
                            World *world = load_world(VultrSource(file.path.string()), engine_global()->component_registry);
                            change_world(world);
                        }
                    }
                }
                auto *texture = is_directory ? Editor::Get()->folder_icon : get_texture_from_file(files[index - sub_directories.size()]);
                texture->Bind(GL_TEXTURE0);
                ImGui::SameLine((200.0 - 125.0) / 2.0);
                ImGui::Image((void *)(intptr_t)texture->GetID(), ImVec2(125, 125));
                double inset = 160 - ImGui::GetFontSize() + ImGui::GetFontSize() / 2;
                ImGui::SameLine(20);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 125);
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 180);
                ImGui::Text("%s", label.c_str());
                ImGui::PopTextWrapPos();

                ImGui::PopID();
                index++;
            }
        }
        ImGui::EndTable();
    }
    ImGui::SameLine(10);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 100);
    Path parent_path;
    for (auto path : current_directory.path)
    {
        parent_path = parent_path / path;
        if (ImGui::Button(path.string().c_str()))
        {
            current_directory = Directory(parent_path);
            current_directory.cached_files = false;
            selected = -1;
            ImGui::End();
            return;
        }
        if (ImGui::BeginDragDropTarget())
        {
            auto *payload = ImGui::AcceptDragDropPayload("File");
            if (payload != nullptr)
            {
                auto *file = static_cast<Vultr::File *>(payload->Data);
                File new_file = Directory(parent_path) / *file;
                Directory dir = Directory(parent_path);
                move_file(*file, dir);
                current_directory.cached_files = false;
                selected = -1;
                ImGui::EndDragDropTarget();
                ImGui::End();
                return;
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        ImGui::Text("/");
        ImGui::SameLine();
    }
    ImGui::SameLine(10);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);

    static bool backed_last_frame = false;
    int back_button_state = glfwGetMouseButton(engine_global()->window, GLFW_MOUSE_BUTTON_4);
    static Path clipboard = "";
    if (ImGui::Button(ICON_FK_BACKWARD) || (back_button_state == GLFW_PRESS && !backed_last_frame))
    {
        backed_last_frame = true;
        if (current_directory.path != get_working_directory())
        {
            auto path = current_directory.path;
            if (path.has_parent_path())
            {
                current_directory = Directory(path.parent_path());
                current_directory.cached_files = false;
                selected = -1;
            }
        }
    }
    else if (back_button_state == GLFW_RELEASE)
    {
        backed_last_frame = false;
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Button("New Folder"))
        {
            std::filesystem::create_directory(current_directory.path / "New Folder");
            current_directory.cached_files = false;
        }

        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::End();
}
