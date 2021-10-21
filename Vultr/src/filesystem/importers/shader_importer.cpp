#include <fstream>
#include <filesystem/importers/shader_importer.h>
#include <core/system_providers/light_system_provider.h>
#include <iostream>
#include <sstream>
#include <stdio.h>

namespace Vultr
{
    template <>
    struct ResourceFinalizeItem<Shader> : IResourceFinalizeItem
    {
        ResourceFinalizeItem(VFileHandle file)
        {
            this->file = file;
        }

        void finalize(void *_resource_manager) override
        {
            printf("Finalizing texture on main thread!\n");
            auto *resource_manager = static_cast<ResourceManager *>(_resource_manager);
            auto *shader = resource_manager->get_asset<Shader>(file);
        }

        ~ResourceFinalizeItem(){};
    };

    template <>
    bool load_resource(const VirtualFilesystem *vfs, VFileHandle file, Shader *resource, ResourceFinalizeItem<Shader> **item)
    {
        assert(vfs_file_exists(vfs, file) && "Cannot load texture, file does not exist!");
        const char *path = vfs->file_table_path.at(file).path;
        printf("Loading texture %s\n", path);

        VFileStream *stream = vfs_open(vfs, file, "rb");

        u64 size = 0;
        auto *buf = vfs_read_full(vfs, &size, stream);
        vfs_close(stream);

        if (buf == nullptr)
        {
            fprintf(stderr, "Failed to load texture %s! Something went wrong opening the file...\n", path);
            return false;
        }

        unsigned char *loaded_buf = TextureImporter::texture_load_memory(resource, buf, size);
        vfs_free_buf(buf);
        if (loaded_buf == nullptr)
        {
            stbi_image_free(loaded_buf);
            fprintf(stderr, "Failed to load texture %s! Something went wrong loading into memory...\n", path);
            return false;
        }

        *item = new ResourceFinalizeItem<Texture>(file, loaded_buf);
        return true;
    }

    template <>
    void free_resource(Texture *resource)
    {
        delete_texture(resource);
    }
    Shader *ShaderImporter::import_shader(const ShaderSource &source)
    {
        Shader shader = create_shader(ShaderSource(source.path));
        return shader;
    }

    Shader *ShaderImporter::import_engine_shader(ShaderProgramSource source)
    {
        // unsigned int program = glCreateProgram();
        // unsigned int vs = compile_shader(GL_VERTEX_SHADER, source.VertexSource);
        // unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, source.FragmentSource);

        // glAttachShader(program, vs);
        // glAttachShader(program, fs);
        // glLinkProgram(program);
        // glValidateProgram(program);

        // glDeleteShader(vs);
        // glDeleteShader(fs);
        // return program;
    }

    // TODO: Eventually, we need to get rid of the STL shit
    ShaderImporter::ShaderProgramSource ShaderImporter::parse_shader(const ShaderSource &source)
    {
        // Opens file
        std::ifstream stream(source.path);

        enum class ShaderType
        {
            NONE = -1,
            VERTEX = 0,
            FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;

        bool preprocessors_set = false;

        while (getline(stream, line))
        {
            // Has found "#shader" on line
            if (line.find("#shader") != std::string::npos)
            {
                // Set mode to vertex
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                // Set mode to fragment
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            // For any other line of code
            else
            {
                // Add line to the appropriate string stream
                ss[(int)type] << line << '\n';
            }
        }
        return {ss[0].str(), ss[1].str()};
    }

    u32 ShaderImporter::create_shader(const ShaderSource &path)
    {
        // Parse the shader
        ShaderProgramSource source = parse_shader(path);
        u32 program = glCreateProgram();
        u32 vs = compile_shader(GL_VERTEX_SHADER, source.VertexSource);
        u32 fs = compile_shader(GL_FRAGMENT_SHADER, source.FragmentSource);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    u32 ShaderImporter::compile_shader(u32 type, const std::string &source)
    {
        unsigned int id = glCreateShader(type);
        auto src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int res;
        glGetShaderiv(id, GL_COMPILE_STATUS, &res);

        if (res == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char *message = new char[length];
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

} // namespace Vultr
