#include <fstream>
#include <helpers/shader_importer.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <helpers/path.h>

namespace Vultr
{
const ShaderProgramSource ShaderImporter::OUTLINE = {
    .VertexSource =
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 normal;\n"
        "layout (location = 2) in vec2 vertextUV; \n"
        "out vec3 FragPos; \n"
        "uniform mat4 model; \n"
        "uniform mat4 view; \n"
        "uniform mat4 projection; \n"
        ""
        "void main() \n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(position, 1.0f); \n"
        "   FragPos = vec3(model * vec4(position, 1.0f)); \n"
        ""
        ""
        "}\n",

    .FragmentSource = "layout (location = 0) in vec3 FragPos;\n"
                      "out vec4 FragColor; \n"
                      "uniform vec4 color; \n"
                      ""
                      "void main() \n"
                      "{\n"
                      "   FragColor = color; \n"
                      ""
                      ""
                      "}\n",
};
Shader *ShaderImporter::ImportShader(const std::string &path)
{
    std::string correct_path = Path::GetFullPath(path);
    Shader *shader = new Shader(CreateShader(correct_path), Deferred);
    return shader;
}

Shader *ShaderImporter::ImportEngineShader(ShaderProgramSource source)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    Shader *shader = new Shader(program, Forward);
    return shader;
}

ShaderProgramSource ShaderImporter::ParseShader(const std::string &filepath)
{
    // Opens file
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
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
unsigned int ShaderImporter::CreateShader(const std::string &path)
{
    // Parse the shader
    ShaderProgramSource source = ParseShader(path);
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.FragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int ShaderImporter::CompileShader(unsigned int type,
                                           const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
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
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertext" : "fragment")
                  << " shader: " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

} // namespace Vultr
