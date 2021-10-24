#include <editor_texture_manager.h>
#include <editor.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>
#include <filesystem/importers/texture_importer.h>

#ifndef SOURCE_PATH
#define SOURCE_PATH "INVALID SOURCE"
#endif

#ifndef _WIN32
INCBIN(folder_icon, SOURCE_PATH "res/folder.png");
INCBIN(file_icon, SOURCE_PATH "res/document.png");
INCBIN(c_icon, SOURCE_PATH "res/c.png");
INCBIN(image_icon, SOURCE_PATH "res/image.png");
INCBIN(shader_icon, SOURCE_PATH "res/s.png");
INCBIN(model_icon, SOURCE_PATH "res/3D.png");
INCBIN(vultr_icon, SOURCE_PATH "res/temp_vultr_icon.png");
#else
INCBIN(folder_icon, "../res/folder.png");
INCBIN(file_icon, "../res/document.png");
INCBIN(c_icon, "../res/c.png");
INCBIN(image_icon, "../res/image.png");
INCBIN(shader_icon, "../res/s.png");
INCBIN(model_icon, "../res/3D.png");
INCBIN(vultr_icon, "../res/temp_vultr_icon.png");
#endif

using namespace Vultr;
EditorTextureManager::EditorTextureManager()
{
    generate_texture(&folder_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&folder_icon, gfolder_icon_data, gfolder_icon_size);

    generate_texture(&file_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&file_icon, gfile_icon_data, gfile_icon_size);

    generate_texture(&c_icon, GL_TEXTURE_2D);

    TextureImporter::texture_import(&c_icon, gc_icon_data, gc_icon_size);

    generate_texture(&image_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&image_icon, gimage_icon_data, gimage_icon_size);

    generate_texture(&shader_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&shader_icon, gshader_icon_data, gshader_icon_size);

    generate_texture(&model_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&model_icon, gmodel_icon_data, gmodel_icon_size);

    generate_texture(&vultr_icon, GL_TEXTURE_2D);
    TextureImporter::texture_import(&vultr_icon, gvultr_icon_data, gvultr_icon_size);
}

Texture &get_texture_from_file(Editor *editor, const IFile *file)
{
    size_t len;
    const char *extension = fextension(file);
    auto *m = &editor->texture_manager;

    if (fextension_matches(extension, FileTypes::SHADER, FileTypes::SHADER_LEN))
    {
        return m->shader_icon;
    }
    else if (fextension_matches(extension, FileTypes::HEADER, FileTypes::HEADER_LEN) || fextension_matches(extension, FileTypes::SOURCE, FileTypes::SOURCE_LEN))
    {
        return m->c_icon;
    }
    else if (fextension_matches(extension, FileTypes::TEXTURE_SOURCE, FileTypes::TEXTURE_SOURCE_LEN))
    {
        return m->image_icon;
    }
    else if (fextension_matches(extension, FileTypes::MODEL_SOURCE, FileTypes::MODEL_SOURCE_LEN))
    {
        return m->model_icon;
    }
    else if (fextension_matches(extension, FileTypes::VULTR_SOURCE, FileTypes::VULTR_SOURCE_LEN))
    {
        return m->vultr_icon;
    }
    else
    {
        return m->file_icon;
    }
}
