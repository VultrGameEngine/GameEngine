#pragma once
#include <vultr.hpp>

struct EditorTextureManager
{
    EditorTextureManager();
    Vultr::Texture *folder_icon;
    Vultr::Texture *file_icon;
    Vultr::Texture *c_icon;
    Vultr::Texture *image_icon;
    Vultr::Texture *shader_icon;
    Vultr::Texture *model_icon;
    Vultr::Texture *vultr_icon;
};
Vultr::Texture *get_texture_from_file(const Vultr::File &file);
