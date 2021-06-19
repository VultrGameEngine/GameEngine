#include <editor_texture_manager.h>
#include <editor.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>
#include <helpers/texture_importer.h>

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
    folder_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gfolder_icon_data, gfolder_icon_size, *folder_icon);

    file_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gfile_icon_data, gfile_icon_size, *file_icon);

    c_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gc_icon_data, gc_icon_size, *c_icon);

    image_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gimage_icon_data, gimage_icon_size, *image_icon);

    shader_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gshader_icon_data, gshader_icon_size, *shader_icon);

    model_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gmodel_icon_data, gmodel_icon_size, *model_icon);

    vultr_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::import(gvultr_icon_data, gvultr_icon_size, *vultr_icon);
}

Texture *get_texture_from_file(const File &file)
{
    auto extension = file_get_extension_type(file);
    auto &m = get_editor().texture_manager;
    switch (extension)
    {
        case File::SHADER:
        {
            return m.shader_icon;
        }
        case File::HEADER:
        case File::SOURCE:
            return m.c_icon;
        case File::TEXTURE:
            return m.image_icon;
        case File::MODEL:
            return m.model_icon;
        case File::VULTR:
            return m.vultr_icon;
        default:
            return m.file_icon;
    }
}
