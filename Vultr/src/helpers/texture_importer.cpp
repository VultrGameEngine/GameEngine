#include <glad/glad.h>
#include <helpers/texture_importer.h>
#include <iostream>
#include <stb_image/stb_image.h>
#include <string>
#include <helpers/path.h>

namespace Vultr
{
    bool TextureImporter::import(Texture &texture, const TextureSource &source)
    {
        stbi_set_flip_vertically_on_load(1);
        s32 width;
        s32 height;
        s32 bpp;
        unsigned char *buffer = stbi_load(source.path.string().c_str(), &width, &height, &bpp, 4);
        if (buffer == nullptr)
        {
            return false;
        }
        TextureImporter::import(texture, buffer, width, height);
        if (buffer)
            stbi_image_free(buffer);
        return true;
    }
    void TextureImporter::import(Texture &texture, const unsigned char *data, u32 size)
    {
        s32 width = 0;
        s32 height = 0;
        unsigned char *buffer = stbi_load_from_memory(data, size, &width, &height, nullptr, 4);

        TextureImporter::import(texture, buffer, width, height);
    }
    void TextureImporter::import(Texture &texture, const unsigned char *data, u32 width, u32 height)
    {
        bind_texture(texture, GL_TEXTURE0);
        texture_parameter_i(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture_parameter_i(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture_parameter_i(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture_parameter_i(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        texture_image_2D(texture, 0, GL_SRGB_ALPHA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    bool TextureImporter::import_skybox(Texture &texture, const std::vector<TextureSource> &paths)
    {
        assert(texture.type == GL_TEXTURE_CUBE_MAP && "Can only write to texture marked as a cube map");

        stbi_set_flip_vertically_on_load(0);
        bind_texture(texture, GL_TEXTURE0);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < paths.size(); i++)
        {
            unsigned char *data = stbi_load(paths[i].path.string().c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                texture_image_2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << paths[i].path << std::endl;
                stbi_image_free(data);
                return false;
            }
        }
        texture_parameter_i(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture_parameter_i(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        texture_parameter_i(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture_parameter_i(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texture_parameter_i(texture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        return true;
    }
} // namespace Vultr
