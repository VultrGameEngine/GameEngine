#pragma once
#include <rendering/models/shader.h>
#include <string>
#include <types/types.hpp>
#include <helpers/file.h>

namespace Vultr::ShaderImporter
{
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };
    const ShaderProgramSource OUTLINE = {
        .VertexSource = "layout (location = 0) in vec3 position;\n"
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
    const ShaderProgramSource EDITOR_INPUT = {
        .VertexSource = "#version 330 core\n"
                        "#extension GL_ARB_separate_shader_objects: enable\n"
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
        .FragmentSource = "#version 330 core\n"
                          "#extension GL_ARB_separate_shader_objects: enable\n"
                          "layout (location = 0) in vec3 FragPos;\n"
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

    Shader *import_shader(const ShaderSource &source);
    Shader *import_engine_shader(ShaderProgramSource source);

    ShaderProgramSource parse_shader(const ShaderSource &source);
    u32 create_shader(const ShaderSource &source);
    u32 compile_shader(u32 type, const std::string &source);

} // namespace Vultr::ShaderImporter
