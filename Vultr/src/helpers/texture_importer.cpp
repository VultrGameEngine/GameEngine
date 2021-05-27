#include <glad/glad.h>
#include <helpers/texture_importer.h>
#include <iostream>
#include <stb_image/stb_image.h>
#include <string>
#include <helpers/path.h>

namespace Vultr
{
    bool TextureImporter::Import(const std::string &path, Texture &texture)
    {
        stbi_set_flip_vertically_on_load(1);
        int width;
        int height;
        int bpp;
        unsigned char *buffer = stbi_load(Path::GetFullPath(path).c_str(), &width, &height, &bpp, 4);
        if (buffer == nullptr)
        {
            return false;
        }
        Import(buffer, texture, width, height);
        if (buffer)
            stbi_image_free(buffer);
        return true;
    }
    void TextureImporter::Import(const unsigned char *data, uint size, Texture &texture)
    {
        int width = 0;
        int height = 0;
        unsigned char *buffer = stbi_load_from_memory(data, size, &width, &height, nullptr, 4);

        Import(buffer, texture, width, height);
    }
    void TextureImporter::Import(const unsigned char *data, Texture &texture, u32 width, u32 height)
    {
        texture.Bind(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    bool TextureImporter::ImportSkybox(const std::vector<std::string> &paths, Texture &texture)
    {
        stbi_set_flip_vertically_on_load(0);
        texture.Bind(GL_TEXTURE0);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < paths.size(); i++)
        {
            unsigned char *data = stbi_load(Path::GetFullPath(paths[i]).c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
                stbi_image_free(data);
                return false;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        return true;
    }
} // namespace Vultr
