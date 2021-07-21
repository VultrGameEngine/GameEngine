#include <windows/asset_browser.h>
#include <fonts/fork_awesome.h>

using namespace Vultr;

static s32 get_num_cols()
{
    // We will assume that the files and directories are 200 in width
#define WIDGET_SIZE 200
    // For padding, we will just remove 2 columns, there is surely a better way to do this but this is fine for now
#define ASSET_BROWSER_SIDE_PADDING 2

    // Get the number of widgets that will fit into our window width
    s32 num_cols = ImGui::GetWindowWidth() / WIDGET_SIZE - ASSET_BROWSER_SIDE_PADDING;

    // We will assume at least 1 column
    if (num_cols < 1)
        num_cols = 1;

    return num_cols;
}

static void draw_directory(const Directory &dir)
{
    // All directories use the folder icon
    auto &texture = get_editor().texture_manager.folder_icon;
    bind_texture(texture, GL_TEXTURE0);
    ImGui::Image((void *)(intptr_t)texture.id, ImVec2(125, 125));
}

static void draw_file(const File &file)
{
    // Different files will have different textures
    auto &texture = get_texture_from_file(file);
    bind_texture(texture, GL_TEXTURE0);
    ImGui::Image((void *)(intptr_t)texture.id, ImVec2(125, 125));
}

#define DESELECT() selected = -1;

static AssetBrowser *&get_asset_browser()
{
    static AssetBrowser *browser = nullptr;
    return browser;
}

static void on_back(void *_state)
{
    // General state things
    auto &state = *static_cast<AssetBrowser *>(_state);
    auto &current_directory = state.current_directory;
    auto &selected = state.selected;

    auto path = current_directory.path;
    if (path.has_parent_path())
    {
        current_directory = Directory(path.parent_path().string());

        // We need to invalidate the current directory cache so that next render it looks correct
        current_directory.cached_files = false;
        DESELECT()
    }
}

static void on_back_button_listener(Engine *e, MouseButtonEvent event)
{
    if (event.button == Input::MOUSE_BUTTON_4 && event.action == GLFW_PRESS)
        on_back(static_cast<void *>(get_asset_browser()));
}

void register_asset_browser(Engine *e, const Vultr::Directory &current_dir)
{
    AssetBrowser *state = new AssetBrowser();
    state->current_directory = current_dir;
    get_asset_browser() = state;
    WindowRenderer renderer = asset_browser_render;
    editor_register_window(e, renderer, static_cast<void *>(state));

    InputSystem::set_mouse_button_listener(e, on_back_button_listener);
}

void asset_browser_render(Engine *e, const Vultr::UpdateTick &tick, void *_state)
{
    ImGui::Begin("Asset Browser");

    // General state things
    auto &state = *static_cast<AssetBrowser *>(_state);
    auto &current_directory = state.current_directory;
    auto &selected = state.selected;
    s32 num_cols = get_num_cols();

    // We are going to automatically use the cache for all of these files unless there is a reason to invalidate the cache, which will be done throughout this loop sometimes
    auto files = directory_get_files(current_directory);
    auto directories = directory_get_sub_directories(current_directory);

    // Set the cursor position down a little bit for padding, we also need this so that we can render the current directory info at the top and whatnot
    // TODO WHAT?? is this a bug or what lmfao
    ImGui::SetCursorPosY(ImGui::GetCursorPosX() + 150);

    // Create the asset list with the number of columns before
    if (ImGui::BeginTable("table1", num_cols))
    {
        // Index into the files and directories.
        s32 index = 0;

        // Keep looping until we've render all of the files and directories
        while (index < files.size() + directories.size())
        {
            // Render each row with the number of columns created before
            ImGui::TableNextRow();
            for (int column = 0; column < num_cols; column++)
            {
                // If the index is over ,then we need to break early, this will occur every time that the files or directories in a row do not reach the end
                if (index >= files.size() + directories.size())
                    break;

                // This is necessary because since we are looping through both the directories and files in one loop, we will eventually pass the directories and move into the files array
                bool is_directory = index < directories.size();
                // Ability to get the indexes easily, so that we don't have to do this every time
                u32 di = index;                      // Directory index
                u32 fi = index - directories.size(); // File index

                // Get the label
                std::string label;
                if (!is_directory)
                {
                    label = file_get_name(files[fi]);
                }
                else
                {
                    label = directory_get_name(directories[di]);
                }

                // ImGui stuff
                ImGui::TableSetColumnIndex(column);
                ImGui::PushID(index);

                // If we selected this element, then we need to update our selection index
                if (ImGui::Selectable("##", selected == index, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(200, 200)))
                {
                    selected = index;
                }

                // Dragging from the file browser
                if (ImGui::BeginDragDropSource())
                {
                    // We are going to copy our file and directory into a new instance and cast it to a void pointer
                    void *payload = is_directory ? static_cast<void *>(new Directory(directories[di])) : static_cast<void *>(new File(files[fi]));

                    // Then send this payload to ImGui
                    ImGui::SetDragDropPayload(is_directory ? "Directory" : "File", payload, sizeof(Vultr::File));

                    // Free our resources
                    free(payload);

                    // Draw our element while dragging
                    if (is_directory)
                    {
                        draw_directory(directories[di]);
                    }
                    else
                    {
                        draw_file(files[fi]);
                    }

                    ImGui::Text("%s", label.c_str());
                    ImGui::EndDragDropSource();
                }

                // Dragging into a folder in the file browser
                if (is_directory && ImGui::BeginDragDropTarget())
                {
                    // If the payload is a file, atm you cannot drag and drop folders
                    const auto *payload = ImGui::AcceptDragDropPayload("File");
                    if (payload != nullptr)
                    {
                        // Get the file
                        auto *file = static_cast<Vultr::File *>(payload->Data);

                        // Then move it into the directory
                        move_file(*file, directories[di]);

                        // We need to invalidate the current directory cache so that next render it looks correct
                        current_directory.cached_files = false;
                        DESELECT();

                        // We will return early so we don't have to worry about a sync issues
                        ImGui::PopID();
                        ImGui::EndTable();
                        ImGui::End();
                        return;
                    }
                    ImGui::EndDragDropTarget();
                }

                // On double click of an element
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                {
                    // If it's a folder, change our current directory
                    if (is_directory)
                    {
                        current_directory = directories[di];

                        // We need to invalidate the current directory cache so that next render it looks correct
                        current_directory.cached_files = false;
                        DESELECT();

                        // We will return early so we don't have to worry about a sync issues
                        ImGui::PopID();
                        ImGui::EndTable();
                        ImGui::End();
                        return;
                    }
                    // If it's a file and it's a Vultr world file, then open it
                    else
                    {
                        auto &file = files[fi];

                        // Make sure that the file extension is .vultr
                        if (file_get_extension_type(file) == File::VULTR)
                        {
                            // Load the world
                            World *world = load_world(e, VultrSource(file.path.string()), e->component_registry);
                            change_world(e, world);
                        }
                    }
                }

                // Draw an icon for our file/dir in the center
                ImGui::SameLine((200.0 - 125.0) / 2.0);

                if (is_directory)
                {
                    draw_directory(directories[di]);
                }
                else
                {
                    draw_file(files[fi]);
                }

                // Draw our text using the label
                ImGui::SameLine(20);

                // Set our cursor position so that it is below our icon
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 125);

                // Wrap text if it doesn't fit
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 180);
                ImGui::Text("%s", label.c_str());
                ImGui::PopTextWrapPos();
                ImGui::PopID();

                // Increase our index
                index++;
            }
        }
        ImGui::EndTable();
    }

    // Render the current path
    ImGui::SameLine(10);

    // This widget will lie above everything else
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 100);

    // This path will sequentially accumulate directories starting from the root path of our current_directory
    Path parent_path;

    // Loop through all of the paths in current_directory
    for (auto path : current_directory.path)
    {
        // Update our parent path
        parent_path = parent_path / path;

        // Clicking on a directory in the path will take you to it
        if (ImGui::Button(path.string().c_str()))
        {
            // Update the current directory
            current_directory = Directory(parent_path.string());

            // We need to invalidate the current directory cache so that next render it looks correct
            current_directory.cached_files = false;
            DESELECT()

            // We will return early so we don't have to worry about a sync issues
            ImGui::End();
            return;
        }

        // You can also drag into the directory path at the top
        if (ImGui::BeginDragDropTarget())
        {
            // Only can accept files
            auto *payload = ImGui::AcceptDragDropPayload("File");
            if (payload != nullptr)
            {
                // Get the file from the payload
                auto *file = static_cast<Vultr::File *>(payload->Data);

                // Create a new file that has its path in the parent_path
                File new_file = Directory(parent_path.string()) / *file;
                Directory dir = Directory(parent_path.string());

                // Move the file to the new location
                move_file(*file, dir);

                // We need to invalidate the current directory cache so that next render it looks correct
                current_directory.cached_files = false;
                DESELECT()

                // We will return early so we don't have to worry about a sync issues
                ImGui::EndDragDropTarget();
                ImGui::End();
                return;
            }
            ImGui::EndDragDropTarget();
        }

        // Draw the little "/" between each directory in the path
        ImGui::SameLine();
        ImGui::Text("/");
        ImGui::SameLine();
    }

    // Right below the path, we are going to show a back button
    ImGui::SameLine(10);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);

    if (ImGui::Button(ICON_FK_BACKWARD))
    {
        on_back(_state);
    }

    // Right click to show context window
    if (ImGui::BeginPopupContextWindow())
    {
        // Create a new folder
        if (ImGui::Button("New Folder"))
        {
            std::filesystem::create_directory(current_directory.path / "New Folder");

            // We need to invalidate the current directory cache so that next render it looks correct
            current_directory.cached_files = false;
            DESELECT()
        }

        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::End();
}
