#include <fstream>
#include <helpers/shader_importer.h>
#include <core/system_providers/light_system_provider.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <helpers/path.h>

namespace Vultr
{
    Shader ShaderImporter::import_shader(const ShaderSource &source)
    {
        auto correct_path = source.path.string();
        Shader shader = create_shader(ShaderSource(correct_path));
        return shader;
    }

    Shader ShaderImporter::import_engine_shader(ShaderProgramSource source)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = compile_shader(GL_VERTEX_SHADER, source.VertexSource);
        unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, source.FragmentSource);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
    }

    // TODO: Eventually, we need to get rid of the STL shit
    ShaderImporter::ShaderProgramSource ShaderImporter::parse_shader(const ShaderSource &source)
    {
        // Opens file
        std::ifstream stream(source.path.string());

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
